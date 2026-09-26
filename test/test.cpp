/*--------------------------------------------------------------------------*/
/*---------------------------- File test.cpp -------------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Unit test of LukFiBlock on the core SMS++ alone.
 *
 * Every one of the 26 test functions of the LukFiBlock is loaded from an
 * in-memory stream and checked for what it is made of, with no Solver on
 * it:
 *
 * - the number of ColVariable of the Block and of active Variable of the
 *   LukFiFunction, the dimension of the functions of fixed dimension being
 *   the tabulated one whatever the stream asks for;
 *
 * - the value at the starting point the LukFiBlock sets, against the one
 *   computed by hand from the definition of the function, and the value at
 *   the optimum of the literature against the tabulated f*, which for a
 *   convex function no point can go below;
 *
 * - the linearization, at the starting point and at random points, some
 *   near it and some far from it: the directional derivative along random
 *   directions has to match a central finite difference, the linearization
 *   of a convex function has to stay below the function at all the other
 *   points, the constant has to give back the value at the point, and the
 *   Range and the Subset forms have to return the same coefficients;
 *
 * - the edges: an unknown function or a negative number of variables
 *   refused by load(), a second load() refused, a function of free
 *   dimension with no variables, the kinks where a subgradient has to be
 *   chosen (the origin of Wolfe, the circle of Mifflin1, the origin of the
 *   functions of absolute values), the continuity of Lewis across its
 *   pieces, the linearizations by name refused, a wrong index in a Subset
 *   refused and a Range clipped to the number of variables;
 *
 * - a change of the values of the ColVariable, and of the parameters of
 *   MaxQR, seen by the next compute(), and the netCDF round trip of the
 *   LukFiBlock, which has to give back the same function.
 *
 * The random points come from fixed seeds. The checks count the failures
 * instead of calling assert(), so they hold under NDEBUG as well; the exit
 * code is 0 when all of them pass, printing "All tests passed!!", and 1
 * otherwise.
 *
 * \author Donato Meoli \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 */
/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <netcdf>

#include "FRealObjective.h"
#include "LukFiBlock.h"

/*--------------------------------------------------------------------------*/
/*-------------------------------- USING -----------------------------------*/
/*--------------------------------------------------------------------------*/

using namespace SMSpp_di_unipi_it;

using LukFiFunction = LukFiBlock::LukFiFunction;

using Vec = std::vector< double >;

/*--------------------------------------------------------------------------*/
/*------------------------------- DATA -------------------------------------*/
/*--------------------------------------------------------------------------*/

namespace {

const double NaN = std::numeric_limits< double >::quiet_NaN();

const double eps = std::numeric_limits< double >::epsilon();

int failures = 0;  // number of failed checks

int budget = 10;   // failed checks still printed for the current case

int unprinted = 0;  // failed checks not printed for the current case

/// one test function: what load() is given, what it has to become, the
/// value at the starting point, the optimum of the literature with its f*
/// and the accuracy the rounded digits of the optimum allow
struct Case {
 int name;           // the number of the function in LukFiBlock
 const char * label;
 int n_load;         // the number of variables given to load()
 int n;              // the number of variables it has to have
 bool convex;
 double f0;          // value at the starting point, NaN if not tabulated
 Vec xstar;          // optimum, empty if not known
 double fstar;       // tabulated optimal value
 double tstar;       // accuracy of the value at the rounded optimum
 };

/*--------------------------------------------------------------------------*/
/*------------------------------ FUNCTIONS ---------------------------------*/
/*--------------------------------------------------------------------------*/

void check( bool ok , const std::string & what )
{
 if( ! ok ) {
  ++failures;
  if( budget > 0 ) {
   --budget;
   std::cout << "FAILED: " << what << std::endl;
   }
  else
   ++unprinted;
  }
 }

/*--------------------------------------------------------------------------*/
/// the end of a case: its name, the outcome and the failures not printed

void report( const std::string & label , int before )
{
 if( unprinted )
  std::cout << "FAILED: " << unprinted << " more checks of " << label
            << std::endl;
 std::cout << std::setw( 14 ) << std::left << label
           << ( failures == before ? "ok" : "KO" ) << std::endl;
 budget = 10;
 unprinted = 0;
 }

/*--------------------------------------------------------------------------*/

std::string str( double v )
{
 std::ostringstream os;
 os << std::setprecision( 12 ) << v;
 return( os.str() );
 }

/*--------------------------------------------------------------------------*/
/// a LukFiBlock loaded from a stream: the function, the number of variables
/// and the ComputeConfig of the LukFiFunction, possibly none

LukFiBlock * make( int name , int n , const std::string & cfg = "" )
{
 auto blk = new LukFiBlock();
 std::istringstream is( std::to_string( name ) + " " + std::to_string( n ) +
			"\n" + cfg );
 try {
  blk->load( is );
  }
 catch( ... ) {
  delete blk;
  throw;
  }
 return( blk );
 }

/*--------------------------------------------------------------------------*/

LukFiFunction * fun( Block * blk )
{
 auto obj = static_cast< FRealObjective * >( blk->get_objective() );
 return( obj ? static_cast< LukFiFunction * >( obj->get_function() )
	     : nullptr );
 }

/*--------------------------------------------------------------------------*/

Vec get_x( LukFiBlock * blk )
{
 Vec x;
 for( auto & v : blk->get_x() )
  x.push_back( v.get_value() );
 return( x );
 }

/*--------------------------------------------------------------------------*/

void set_x( LukFiBlock * blk , const Vec & x )
{
 auto & v = blk->get_x();
 for( size_t i = 0 ; i < x.size() ; ++i )
  v[ i ].set_value( x[ i ] );
 }

/*--------------------------------------------------------------------------*/
/// the value at x, computed anew

double value( LukFiBlock * blk , const Vec & x )
{
 set_x( blk , x );
 auto f = fun( blk );
 f->compute();
 return( f->get_value() );
 }

/*--------------------------------------------------------------------------*/
/// the value at x and the coefficients of the linearization there

double linearize( LukFiBlock * blk , const Vec & x , Vec & g )
{
 const double v = value( blk , x );
 g.assign( x.size() , NaN );
 fun( blk )->get_linearization_coefficients( g.data() );
 return( v );
 }

/*--------------------------------------------------------------------------*/

double dot( const Vec & a , const Vec & b )
{
 double s = 0;
 for( size_t i = 0 ; i < a.size() ; ++i )
  s += a[ i ] * b[ i ];
 return( s );
 }

/*--------------------------------------------------------------------------*/
/// sum of the absolute values of the products, the scale of dot( a , b )

double adot( const Vec & a , const Vec & b )
{
 double s = 0;
 for( size_t i = 0 ; i < a.size() ; ++i )
  s += std::abs( a[ i ] * b[ i ] );
 return( s );
 }

/*--------------------------------------------------------------------------*/

double infnorm( const Vec & a )
{
 double s = 0;
 for( auto v : a )
  s = std::max( s , std::abs( v ) );
 return( s );
 }

/*--------------------------------------------------------------------------*/
/// the directional derivative g' d against a central difference, with a
/// step 1000 times shorter too to rule out a kink met by the longer one; at
/// a kink, where the one-sided differences disagree, the linearization of a
/// convex function has to lie between them, while nothing is asked of a
/// nonconvex one

bool fd_matches( LukFiBlock * blk , const Vec & x , const Vec & g ,
		 const Vec & d , double fx , bool convex , std::string & msg )
{
 const double gd = dot( g , d );
 const double scale = 1 + adot( g , d );
 double fwd = NaN , bwd = NaN , ctr = NaN , noise = 0;
 for( double h = 1e-6 * ( 1 + infnorm( x ) ) , k = 0 ; k < 2 ;
      h /= 1000 , ++k ) {
  Vec xp( x ) , xm( x );
  for( size_t i = 0 ; i < x.size() ; ++i ) {
   xp[ i ] += h * d[ i ];
   xm[ i ] -= h * d[ i ];
   }
  const double fp = value( blk , xp );
  const double fm = value( blk , xm );
  noise = 8 * eps * ( 1 + std::abs( fx ) ) / h;
  ctr = ( fp - fm ) / ( 2 * h );
  if( std::abs( ctr - gd ) <= 1e-5 * scale + noise )
   return( true );
  fwd = ( fp - fx ) / h;
  bwd = ( fx - fm ) / h;
  }

 msg = "g'd = " + str( gd ) + ", central difference = " + str( ctr ) +
       ", one-sided " + str( bwd ) + " and " + str( fwd );
 if( std::abs( fwd - bwd ) <= 1e-3 * scale + noise )  // smooth: wrong g
  return( false );
 if( ! convex )
  return( true );
 const double tol = 1e-4 * scale + noise;
 return( ( bwd - tol <= gd ) && ( gd <= fwd + tol ) );
 }

/*--------------------------------------------------------------------------*/
/// every check of the linearization at the given points; for a convex
/// function the linearization at each point has to stay below the function
/// at all the others

void check_linearizations( LukFiBlock * blk , const Case & c ,
			   const std::vector< Vec > & pts , std::mt19937 & rg )
{
 std::uniform_real_distribution<> u( -1 , 1 );
 const auto f = fun( blk );
 const auto n = c.n;

 std::vector< double > fv( pts.size() );
 for( size_t p = 0 ; p < pts.size() ; ++p )
  fv[ p ] = value( blk , pts[ p ] );

 for( size_t p = 0 ; p < pts.size() ; ++p ) {
  const auto & x = pts[ p ];
  const std::string at = std::string( c.label ) + ", point " +
                         std::to_string( p );
  Vec g;
  const double fx = linearize( blk , x , g );

  bool finite = std::isfinite( fx );
  for( auto gi : g )
   finite &= std::isfinite( gi );
  check( finite , at + ": value or subgradient not finite" );
  if( ! finite )
   continue;

  // the constant gives back the value at the point
  const double alpha = f->get_linearization_constant();
  check( std::abs( alpha + dot( g , x ) - fx ) <=
	 1e-12 * ( 1 + std::abs( fx ) + adot( g , x ) ) ,
	 at + ": constant " + str( alpha ) + " + g'x = " +
	 str( alpha + dot( g , x ) ) + " instead of f(x) = " + str( fx ) );

  // the Subset form, in reverse order, and a Range inside the variables
  // return the same coefficients as the whole Range
  Block::Subset sub( n );
  for( int i = 0 ; i < n ; ++i )
   sub[ i ] = n - 1 - i;
  Vec gs( n , NaN );
  f->get_linearization_coefficients( gs.data() , sub );
  bool same = true;
  for( int i = 0 ; i < n ; ++i )
   same &= ( gs[ i ] == g[ n - 1 - i ] );
  check( same , at + ": the Subset form differs from the Range one" );

  if( n > 2 ) {
   Vec gr( n - 2 , NaN );
   f->get_linearization_coefficients( gr.data() ,
				      Block::Range( 1 , n - 1 ) );
   same = true;
   for( int i = 1 ; i < n - 1 ; ++i )
    same &= ( gr[ i - 1 ] == g[ i ] );
   check( same , at + ": a partial Range differs from the whole one" );
   }

  // the directional derivative matches the finite difference
  for( int k = 0 ; k < 2 ; ++k ) {
   Vec d( n );
   for( auto & di : d )
    di = u( rg );
   std::string msg;
   const bool ok = fd_matches( blk , x , g , d , fx , c.convex , msg );
   check( ok , at + ", direction " + std::to_string( k ) + ": " + msg );
   }

  // a convex function stays above the linearization
  if( c.convex )
   for( size_t q = 0 ; q < pts.size() ; ++q ) {
    if( q == p )
     continue;
    Vec dx( n );
    for( int i = 0 ; i < n ; ++i )
     dx[ i ] = pts[ q ][ i ] - x[ i ];
    const double lin = fx + dot( g , dx );
    check( fv[ q ] >= lin - 1e-9 * ( 1 + std::abs( fx ) +
				     std::abs( fv[ q ] ) + adot( g , dx ) ) ,
	   at + ": f at point " + std::to_string( q ) + " = " +
	   str( fv[ q ] ) + " is below the linearization " + str( lin ) );
    }
  }
 }

/*--------------------------------------------------------------------------*/
/// one test function: dimension, value at the start and at the optimum,
/// linearizations at the start, near it, far from it and near the optimum

void test_function( const Case & c )
{
 const int before = failures;
 std::mt19937 rg( 1000 + c.name );
 std::uniform_real_distribution<> u( -1 , 1 );

 std::string cfg;
 if( c.name == 25 )  // MaxQR: a few components rather than the only one
  cfg = "0 2 intNrCmp 5 intseed 3 0 0 0 0 0 *\n";
 auto blk = make( c.name , c.n_load , cfg );
 const auto f = fun( blk );

 check( int( blk->get_x().size() ) == c.n , std::string( c.label ) + ": " +
	std::to_string( blk->get_x().size() ) + " ColVariable instead of " +
	std::to_string( c.n ) );
 check( int( blk->get_number_static_variables() ) == 1 ,
	std::string( c.label ) + ": the ColVariable are not one static group" );
 check( int( f->get_num_active_var() ) == c.n , std::string( c.label ) +
	": " + std::to_string( f->get_num_active_var() ) +
	" active Variable instead of " + std::to_string( c.n ) );
 for( int i = 0 ; i < c.n ; ++i )
  if( f->is_active( & blk->get_x()[ i ] ) != Block::Index( i ) ) {
   check( false , std::string( c.label ) + ": ColVariable " +
	  std::to_string( i ) + " is not active variable " +
	  std::to_string( i ) );
   break;
   }
 if( c.convex )
  check( f->is_convex() , std::string( c.label ) + ": not said convex" );

 const Vec x0 = get_x( blk );

 // the value at the starting point
 if( ! std::isnan( c.f0 ) ) {
  const double v = value( blk , x0 );
  check( std::abs( v - c.f0 ) <= 1e-9 * ( 1 + std::abs( c.f0 ) ) ,
	 std::string( c.label ) + ": f(x0) = " + str( v ) + " instead of " +
	 str( c.f0 ) );
  }

 // the starting point of TR48 is a point close to the optimum, less than
 // 1e-4 |f*| above it
 if( c.name == 16 ) {
  const double v = value( blk , x0 );
  check( ( v >= c.fstar - 1e-6 * std::abs( c.fstar ) ) &&
	 ( v <= c.fstar + 1e-4 * std::abs( c.fstar ) ) ,
	 std::string( c.label ) + ": f(x0) = " + str( v ) + " is not close "
	 "above f* = " + str( c.fstar ) );
  }

 // the value at the optimum, which a convex function cannot go below but
 // for the rounding of the tabulated f*
 if( ! c.xstar.empty() ) {
  const double v = value( blk , c.xstar );
  check( std::abs( v - c.fstar ) <= c.tstar , std::string( c.label ) +
	 ": f(x*) = " + str( v ) + " instead of " + str( c.fstar ) );
  if( c.convex )
   check( v >= c.fstar - 1e-6 * ( 1 + std::abs( c.fstar ) ) ,
	  std::string( c.label ) + ": f(x*) = " + str( v ) +
	  " is below f* = " + str( c.fstar ) );
  }

 // the points: the start, three near it, five far from it, and two near
 // the optimum
 std::vector< Vec > pts = { x0 };
 for( int k = 0 ; k < 3 ; ++k ) {
  Vec x( x0 );
  for( auto & xi : x )
   xi += 0.1 * u( rg );
  pts.push_back( x );
  }
 for( int k = 0 ; k < 5 ; ++k ) {
  Vec x( x0 );
  for( auto & xi : x )
   xi += 2 * ( 1 + std::abs( xi ) ) * u( rg );
  pts.push_back( x );
  }
 // a point in each piece of the functions defined by pieces
 if( c.name == 10 )  // Wolfe
  for( const Vec & x : std::vector< Vec >{ { 1 , 2 } , { 0.5 , -1.5 } ,
					   { -1 , 0.5 } , { -0.5 , -2 } } )
   pts.push_back( x );
 if( c.name == 26 )  // Lewis
  for( const Vec & x : std::vector< Vec >{ { 2 , -1 } , { 2 , 1 } ,
					   { 1 , 2 } , { 0.5 , 3 } } )
   pts.push_back( x );
 if( ! c.xstar.empty() )
  for( int k = 0 ; k < 2 ; ++k ) {
   Vec x( c.xstar );
   for( auto & xi : x )
    xi += 1e-3 * u( rg );
   pts.push_back( x );
   }

 // MaxQR is the max of b_j || x - c_j || + a_j, whose gradient is
 // b_j ( x - c_j ) / || x - c_j ||; its norm is b_j whatever x
 if( c.name == 25 ) {
  Vec g;
  linearize( blk , x0 , g );
  const double gn = std::sqrt( dot( g , g ) );
  check( gn <= 100 * ( 1 + 1e-12 ) ,
	 "MaxQR: || g || = " + str( gn ) + " at x0, while the gradient of "
	 "b_j || x - c_j || + a_j, b_j in [ 0 , 100 ], has norm b_j: the "
	 "coefficients are 2 b_j ( x - c_j ), the gradient of "
	 "b_j || x - c_j ||^2" );
  }

 check_linearizations( blk , c , pts , rg );

 delete blk;
 report( c.label , before );
 }

/*--------------------------------------------------------------------------*/
/// load() refuses what it cannot build

void test_load_refusals( void )
{
 const int before = failures;

 // the functions are numbered from 1 to 26
 for( int name : { 0 , 27 , -3 } ) {
  bool thrown = false;
  try {
   delete make( name , 2 );
   }
  catch( std::invalid_argument & ) { thrown = true; }
  check( thrown , "load() accepts the unknown function " +
	 std::to_string( name ) );
  }

 // a negative number of variables
 {
  bool thrown = false;
  try {
   delete make( 23 , -1 );
   }
  catch( std::invalid_argument & ) { thrown = true; }
  check( thrown , "load() accepts -1 variables" );
  }

 // a second load() on the same LukFiBlock
 {
  auto blk = make( 1 , 2 );
  bool thrown = false;
  try {
   std::istringstream is( "1 2" );
   blk->load( is );
   }
  catch( std::logic_error & ) { thrown = true; }
  check( thrown , "a second load() is accepted" );
  delete blk;
  }

 // the factory builds a LukFiBlock that loads as one built directly
 {
  auto blk = Block::new_Block( "LukFiBlock" );
  check( dynamic_cast< LukFiBlock * >( blk ) != nullptr ,
	 "the factory does not give a LukFiBlock" );
  if( blk ) {
   std::istringstream is( "4 2" );
   blk->load( is );
   auto f = fun( blk );
   f->compute();
   check( f->get_value() == 20 , "CB3 from the factory: f(x0) = " +
	  str( f->get_value() ) );
   delete blk;
   }
  }
 report( "load refusals" , before );
 }

/*--------------------------------------------------------------------------*/
/// the kinks, the empty function and the arguments the linearization
/// refuses

void test_edges( void )
{
 const int before = failures;

 // a function of free dimension with no variables: value 0, nothing to
 // write in the linearization
 {
  auto blk = make( 23 , 0 );
  auto f = fun( blk );
  check( blk->get_x().empty() , "smooth with 0 variables has some" );
  f->compute();
  check( f->get_value() == 0 , "smooth with 0 variables: f = " +
	 str( f->get_value() ) );
  double sentinel = 42;
  f->get_linearization_coefficients( & sentinel );
  check( sentinel == 42 , "smooth with 0 variables writes a coefficient" );
  check( f->get_linearization_constant() == 0 ,
	 "smooth with 0 variables: the constant is not 0" );
  delete blk;
  }

 // Wolfe at the origin, the only point where the square root vanishes:
 // both forms give a finite subgradient, and it is a subgradient of the
 // convex function, f( y ) >= g' y at every y
 {
  auto blk = make( 10 , 2 );
  auto f = fun( blk );
  Vec g;
  const double f0 = linearize( blk , { 0 , 0 } , g );
  Vec gs( 2 , NaN );
  Block::Subset sub = { 0 , 1 };
  f->get_linearization_coefficients( gs.data() , sub );
  check( f0 == 0 , "Wolfe at the origin: f = " + str( f0 ) );
  check( std::isfinite( g[ 0 ] ) && std::isfinite( g[ 1 ] ) ,
	 "Wolfe at the origin: Range subgradient not finite" );
  check( std::isfinite( gs[ 0 ] ) && std::isfinite( gs[ 1 ] ) ,
	 "Wolfe at the origin: Subset subgradient not finite" );
  for( const Vec & y : std::vector< Vec >{ { -1 , 0 } , { 1 , 0 } ,
					   { 0.5 , 1 } , { -0.5 , -1 } ,
					   { 1 , 1 } , { 1 , -0.3 } } ) {
   const double fy = value( blk , y );
   check( fy >= dot( g , y ) - 1e-12 , "Wolfe at the origin: f( " +
	  str( y[ 0 ] ) + " , " + str( y[ 1 ] ) + " ) = " + str( fy ) +
	  " is below the linearization " + str( dot( g , y ) ) );
   }
  delete blk;
  }

 // the other kinks of convex functions at their optimum: the circle of
 // Mifflin1, the origin of Maxl, Goffin, MXHILB, L1HILB and AbsVal; the
 // linearization there stays below the function at random points
 for( int name : { 8 , 15 , 20 , 21 , 22 , 24 } ) {
  auto blk = make( name , 6 );
  const auto n = blk->get_x().size();
  Vec xs( n , 0 );
  if( name == 8 )
   xs[ 0 ] = 1;
  Vec g;
  const double fs = linearize( blk , xs , g );
  std::mt19937 rg( 7 + name );
  std::uniform_real_distribution<> u( -1 , 1 );
  for( int k = 0 ; k < 20 ; ++k ) {
   Vec y( xs );
   for( auto & yi : y )
    yi += u( rg );
   Vec dy( n );
   for( size_t i = 0 ; i < n ; ++i )
    dy[ i ] = y[ i ] - xs[ i ];
   const double fy = value( blk , y );
   check( fy >= fs + dot( g , dy ) - 1e-12 * ( 1 + std::abs( fy ) ) ,
	  "function " + std::to_string( name ) + " at its kink: f( y ) = " +
	  str( fy ) + " is below the linearization " +
	  str( fs + dot( g , dy ) ) );
   }
  delete blk;
  }

 // Lewis is continuous across the curves that separate its pieces:
 // x_1 = 0, x_1 = x_0^2, x_1 = 4 x_0^2
 {
  auto blk = make( 26 , 2 );
  const double d = 1e-9;
  for( double t : { -2.0 , -0.5 , 0.7 , 3.0 } )
   for( double c : { 0.0 , 1.0 , 4.0 } ) {
    const double y = c * t * t;
    const double lo = value( blk , { t , y - d } );
    const double hi = value( blk , { t , y + d } );
    check( std::abs( hi - lo ) <= 1e-6 , "Lewis jumps at x = ( " + str( t ) +
	   " , " + str( y ) + " ): " + str( lo ) + " and " + str( hi ) );
    }
  delete blk;
  }

 // the linearizations by name, a wrong index in a Subset, a Range beyond
 // the variables and an empty Range
 {
  auto blk = make( 12 , 5 );
  auto f = fun( blk );
  Vec g;
  linearize( blk , get_x( blk ) , g );

  bool thrown = false;
  try {
   Vec h( 5 );
   f->get_linearization_coefficients( h.data() , Block::INFRange , 0 );
   }
  catch( std::logic_error & ) { thrown = true; }
  check( thrown , "a linearization by name is given (Range form)" );

  thrown = false;
  try {
   Vec h( 5 );
   Block::Subset sub = { 0 , 1 };
   f->get_linearization_coefficients( h.data() , sub , false , 0 );
   }
  catch( std::logic_error & ) { thrown = true; }
  check( thrown , "a linearization by name is given (Subset form)" );

  thrown = false;
  try {
   f->get_linearization_constant( 0 );
   }
  catch( std::logic_error & ) { thrown = true; }
  check( thrown , "a linearization constant by name is given" );

  thrown = false;
  try {
   Vec h( 5 );
   Block::Subset sub = { 1 , 5 };
   f->get_linearization_coefficients( h.data() , sub );
   }
  catch( std::invalid_argument & ) { thrown = true; }
  check( thrown , "the index 5 of 5 variables is accepted in a Subset" );

  Vec h( 8 , 42 );
  f->get_linearization_coefficients( h.data() , Block::Range( 2 , 100 ) );
  bool ok = ( h[ 3 ] == 42 );
  for( int i = 0 ; i < 3 ; ++i )
   ok &= ( h[ i ] == g[ i + 2 ] );
  check( ok , "a Range beyond the variables is not clipped to them" );

  Vec e( 1 , 42 );
  f->get_linearization_coefficients( e.data() , Block::Range( 3 , 3 ) );
  check( e[ 0 ] == 42 , "an empty Range writes a coefficient" );
  delete blk;
  }

 report( "edges" , before );
 }

/*--------------------------------------------------------------------------*/
/// a change of the ColVariable, and of the parameters of MaxQR, is seen by
/// the next compute()

void test_changes( void )
{
 const int before = failures;

 // the value follows the ColVariable
 {
  auto blk = make( 1 , 2 );
  auto f = fun( blk );
  f->compute();
  check( f->get_value() == 24.2 || std::abs( f->get_value() - 24.2 ) < 1e-12 ,
	 "Rosenbrock at the start: " + str( f->get_value() ) );
  blk->get_x()[ 0 ].set_value( 1 );
  blk->get_x()[ 1 ].set_value( 1 );
  f->compute();
  check( f->get_value() == 0 , "Rosenbrock after moving to ( 1 , 1 ): " +
	 str( f->get_value() ) );
  blk->get_x()[ 1 ].set_value( 0 );
  f->compute();
  check( f->get_value() == 100 , "Rosenbrock after moving to ( 1 , 0 ): " +
	 str( f->get_value() ) );
  delete blk;
  }

 // MaxQR takes its parameters from the ComputeConfig of load() and from
 // set_par(), and its value changes with them and comes back with them
 {
  auto blk = make( 25 , 4 , "0 2 intNrCmp 4 intseed 11 0 0 0 0 0 *\n" );
  auto f = fun( blk );
  check( f->get_int_par( LukFiFunction::intNrCmp ) == 4 ,
	 "MaxQR: intNrCmp " +
	 std::to_string( f->get_int_par( LukFiFunction::intNrCmp ) ) +
	 " instead of the 4 of the ComputeConfig" );
  check( f->get_int_par( LukFiFunction::intseed ) == 11 ,
	 "MaxQR: intseed " +
	 std::to_string( f->get_int_par( LukFiFunction::intseed ) ) +
	 " instead of the 11 of the ComputeConfig" );
  check( f->int_par_str2idx( "intseed" ) == LukFiFunction::intseed &&
	 f->int_par_idx2str( LukFiFunction::intNrCmp ) == "intNrCmp" ,
	 "MaxQR: the names of the parameters" );

  const Vec x0 = get_x( blk );
  const double v11 = value( blk , x0 );
  f->set_par( LukFiFunction::intseed , 12 );
  const double v12 = value( blk , x0 );
  check( v12 != v11 , "MaxQR: another seed gives the same value " +
	 str( v11 ) );
  f->set_par( LukFiFunction::intseed , 11 );
  check( value( blk , x0 ) == v11 , "MaxQR: the seed back does not give "
	 "the value back" );

  // one more component (the first four being the same) cannot lower it
  f->set_par( LukFiFunction::intNrCmp , 5 );
  const double v5 = value( blk , x0 );
  check( v5 >= v11 , "MaxQR: a fifth component lowers the max from " +
	 str( v11 ) + " to " + str( v5 ) );
  delete blk;
  }

 report( "changes" , before );
 }

/*--------------------------------------------------------------------------*/
/// the netCDF round trip gives back the same function: the same number of
/// variables and the same values, i.e., the same function with the same
/// parameters; the ComputeConfig of the LukFiFunction is checked in the
/// file, the pairs of pars being the parameters not at their default

void round_trip( int name , int n , const std::string & cfg ,
		 const std::vector< std::pair< std::string , int > > & pars )
{
 const std::string what = "round trip of function " + std::to_string( name );
 const std::string file = "LukFiBlock_unit_test.nc4";
 auto blk = make( name , n , cfg );
 Block * back = nullptr;
 try {
  {
   netCDF::NcFile out( file , netCDF::NcFile::replace ,
		       netCDF::NcFile::nc4 );
   blk->serialize( out );
   }
  netCDF::NcFile in( file , netCDF::NcFile::read );

  // the parameters not at their default, and only them, are in the file
  auto cg = in.getGroup( "lukfi_config" );
  check( cg.isNull() == pars.empty() , what + ( pars.empty() ?
	 ": a ComputeConfig with all at the default is written" :
	 ": no ComputeConfig is written" ) );
  if( ! cg.isNull() ) {
   auto nd = cg.getDim( "int_par_num" );
   const size_t num = nd.isNull() ? 0 : nd.getSize();
   check( num == pars.size() , what + ": " + std::to_string( num ) +
	  " int parameters written instead of " +
	  std::to_string( pars.size() ) );
   if( num == pars.size() ) {
    auto names = cg.getVar( "int_par_names" );
    auto vals = cg.getVar( "int_par_vals" );
    for( size_t i = 0 ; i < num ; ++i ) {
     char * p = nullptr;
     int v = 0;
     names.getVar( { i } , & p );
     vals.getVar( { i } , & v );
     const std::string nm( p ? p : "" );
     nc_free_string( 1 , & p );
     check( ( nm == pars[ i ].first ) && ( v == pars[ i ].second ) ,
	    what + ": parameter " + nm + " = " + std::to_string( v ) +
	    " written instead of " + pars[ i ].first + " = " +
	    std::to_string( pars[ i ].second ) );
     }
    }
   }

  // reading a ComputeConfig back from netCDF is the job of the core, the
  // same for every Block: the LukFiBlock is read back when the file has none
  if( pars.empty() )
   back = Block::new_Block( in );
  }
 catch( std::exception & e ) {
  check( false , what + ": " + e.what() );
  }

 if( ! pars.empty() ) {
  delete blk;
  std::remove( file.c_str() );
  return;
  }

 auto lb = dynamic_cast< LukFiBlock * >( back );
 check( lb != nullptr , what + ": no LukFiBlock is read back" );
 if( lb ) {
  check( lb->get_x().size() == blk->get_x().size() , what + ": " +
	 std::to_string( lb->get_x().size() ) + " variables instead of " +
	 std::to_string( blk->get_x().size() ) );
  check( fun( lb ) != nullptr , what + ": the Block read back has no "
	 "Objective" );
  check( lb->get_number_static_variables() == 1 , what + ": the Block read "
	 "back has no static ColVariable" );
  if( fun( lb ) && ( lb->get_x().size() == blk->get_x().size() ) ) {
   const Vec x0 = get_x( blk );
   check( get_x( lb ) == x0 , what + ": another starting point" );
   std::mt19937 rg( 99 );
   std::uniform_real_distribution<> u( -1 , 1 );
   for( int k = 0 ; k < 3 ; ++k ) {
    Vec x( x0 );
    if( k )
     for( auto & xi : x )
      xi += u( rg );
    const double v = value( blk , x );
    const double w = value( lb , x );
    check( v == w , what + ": f = " + str( w ) + " instead of " + str( v ) );
    }
   }
  }
 delete back;
 delete blk;
 std::remove( file.c_str() );
 }

/*--------------------------------------------------------------------------*/

void test_round_trips( void )
{
 const int before = failures;
 // all the parameters at their default: no ComputeConfig in the file
 round_trip( 12 , 5 , "" , {} );
 round_trip( 23 , 3 , "" , {} );
 round_trip( 25 , 4 , "" , {} );
 // some parameters changed: they are in the file
 round_trip( 25 , 4 , "0 2 intNrCmp 4 intseed 11 0 0 0 0 0 *\n" ,
	     { { "intNrCmp" , 4 } , { "intseed" , 11 } } );
 report( "round trips" , before );
 }

}  // end( namespace )

/*--------------------------------------------------------------------------*/
/*--------------------------------- MAIN -----------------------------------*/
/*--------------------------------------------------------------------------*/

int main( void )
{
 // the sum of the harmonic numbers 1 / ( i + j - 1 ), i , j = 1 ... 50,
 // i.e., the value of L1HILB at the all-ones start, counted by diagonals
 double l1hilb0 = 0;
 for( int k = 1 ; k < 100 ; ++k )
  l1hilb0 += double( std::min( k , 100 - k ) ) / k;
 double h50 = 0;  // the first row, i.e., the value of MXHILB
 for( int k = 1 ; k <= 50 ; ++k )
  h50 += 1.0 / k;

 const double s2 = std::sqrt( 2.0 );

 const std::vector< Case > cases = {
  //  n.  label        load  n  cvx   f(x0)
  { 1 , "Rosenbrock" ,  7 ,  2 , false , 24.2 ,
    { 1 , 1 } , 0 , 1e-12 } ,
  { 2 , "Crescent" ,    7 ,  2 , false , 4.25 ,
    { 0 , 0 } , 0 , 1e-12 } ,
  { 3 , "CB2" ,         7 ,  2 , true , 5.41 ,
    { 1.139286 , 0.899365 } , 1.9522245 , 1e-5 } ,
  { 4 , "CB3" ,         7 ,  2 , true , 20 ,
    { 1 , 1 } , 2 , 1e-12 } ,
  { 5 , "DEM" ,         7 ,  2 , true , 6 ,
    { 0 , -3 } , -3 , 1e-12 } ,
  { 6 , "QL" ,          7 ,  2 , true , 56 ,
    { 1.2 , 2.4 } , 7.2 , 1e-12 } ,
  { 7 , "LQ" ,          7 ,  2 , true , 1 ,
    { 1 / s2 , 1 / s2 } , - s2 , 1e-12 } ,
  { 8 , "Mifflin1" ,    7 ,  2 , true , -0.8 ,
    { 1 , 0 } , -1 , 1e-12 } ,
  { 9 , "Mifflin2" ,    7 ,  2 , false , 4.75 ,
    { 1 , 0 } , -1 , 1e-12 } ,
  { 10 , "Wolfe" ,      7 ,  2 , true , 5 * std::sqrt( 145.0 ) ,
    { -1 , 0 } , -8 , 1e-12 } ,
  { 11 , "Rosen-Suzuki" , 7 , 4 , true , 0 ,
    { 0 , 1 , 2 , -1 } , -44 , 1e-12 } ,
  { 12 , "Shor" ,       7 ,  5 , true , 80 ,
    { 1.124361 , 0.979450 , 1.477706 , 0.920234 , 1.124335 } ,
    22.600162 , 1e-3 } ,
  { 13 , "Maxquad" ,    7 , 10 , true , NaN ,
    { -0.1263 , -0.0344 , -0.0068 , 0.0264 , 0.0673 ,
      -0.2784 ,  0.0742 ,  0.1385 , 0.0840 , 0.0386 } ,
    -0.8414083 , 1e-3 } ,
  { 14 , "Maxq" ,       7 , 20 , true , 400 ,
    Vec( 20 , 0 ) , 0 , 1e-12 } ,
  { 15 , "Maxl" ,       7 , 20 , true , 20 ,
    Vec( 20 , 0 ) , 0 , 1e-12 } ,
  { 16 , "TR48" ,       7 , 48 , true , NaN ,
    {} , -638565 , 1 } ,
  { 17 , "Colville1" ,  7 ,  5 , false , 20 ,
    { 0.3 , 0.3335 , 0.4 , 0.4283 , 0.224 } , -32.348679 , 5e-2 } ,
  { 18 , "HS78" ,       7 ,  5 , false , 72.75 ,
    { -1.717143 , 1.595709 , 1.827247 , -0.7636413 , -0.7636450 } ,
    -2.9197004 , 1e-4 } ,
  { 19 , "Gill" ,       7 , 10 , false , NaN ,
    {} , 9.7857 , 1e-4 } ,
  { 20 , "Goffin" ,     7 , 50 , true , 1225 ,
    Vec( 50 , 0 ) , 0 , 1e-12 } ,
  { 21 , "MXHILB" ,     7 , 50 , true , h50 ,
    Vec( 50 , 0 ) , 0 , 1e-12 } ,
  { 22 , "L1HILB" ,     7 , 50 , true , l1hilb0 ,
    Vec( 50 , 0 ) , 0 , 1e-12 } ,
  { 23 , "smooth" ,     6 ,  6 , true , 3 ,
    Vec( 6 , 0 ) , 0 , 1e-12 } ,
  { 24 , "AbsVal" ,     6 ,  6 , true , 6 ,
    Vec( 6 , 0 ) , 0 , 1e-12 } ,
  { 25 , "MaxQR" ,      6 ,  6 , true , NaN ,
    {} , 0 , 0 } ,
  { 26 , "Lewis" ,      7 ,  2 , false , 110 ,
    { 0 , 0 } , 0 , 1e-12 }
  };

 try {
  for( const auto & c : cases )
   test_function( c );
  test_load_refusals();
  test_edges();
  test_changes();
  test_round_trips();
  }
 catch( std::exception & e ) {
  check( false , std::string( "exception: " ) + e.what() );
  }

 if( failures )
  std::cout << failures << " checks FAILED!!" << std::endl;
 else
  std::cout << "All tests passed!!" << std::endl;
 return( failures ? 1 : 0 );
 }

/*--------------------------------------------------------------------------*/
/*------------------------- End File test.cpp ------------------------------*/
/*--------------------------------------------------------------------------*/
