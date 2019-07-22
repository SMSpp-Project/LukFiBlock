/*--------------------------------------------------------------------------*/
/*------------------------- File LukFiBlock.cpp ----------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Implementation of the LukFiBlock class.
 *
 * \version 0.01
 *
 * \date 16 - 07 - 2019
 *
 * \author Antonio Frangioni \n
 *         Operations Research Group \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \author Enrico Gorgone \n
 *         Operations Research Group \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * Copyright &copy by Antonio Frangioni
 */
/*--------------------------------------------------------------------------*/
/*---------------------------- IMPLEMENTATION ------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include "LukFiBlock.h"

#include <iostream>
#include <numeric>

/*--------------------------------------------------------------------------*/
/*------------------------- NAMESPACE AND USING ----------------------------*/
/*--------------------------------------------------------------------------*/

using namespace SMSpp_di_unipi_it;
using namespace std;

/*--------------------------------------------------------------------------*/
/*------------------------------ MACROS ------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*----------------------------- FUNCTIONS ----------------------------------*/
/*--------------------------------------------------------------------------*/

template< typename T>
static void read_T( istream & iStrm , T & t )
{
 iStrm >> eatcomments;
 int c = iStrm.peek();

 switch( c ) {
  case 'I' :
  case 'i' : t = Inf<T>();
             break;
  case '-' : iStrm.get();
             read_T( iStrm , t );
             t = - t;
             return;
  case 'M' :
  case 'm' : t = -Inf<T>();
             break;
  default :  iStrm >> t;
             return;
  }

 do { c = iStrm.get(); c = iStrm.peek();
  } while( ( c != iStrm.widen( ' ' ) ) &&
	   ( c != iStrm.widen( '\n' ) ) &&
	   ( c != iStrm.widen( '\t' ) ) );
 }

/*--------------------------------------------------------------------------*/

static inline int read_int( istream & iStrm )
{
 int d;
 read_T( iStrm , d );
 return( d );
 }

/*--------------------------------------------------------------------------*/

static inline double read_dbl( istream & iStrm )
{
 double d;
 read_T( iStrm , d );
 return( d );
 }

/*--------------------------------------------------------------------------*/
/*----------------------------- STATIC MEMBERS -----------------------------*/
/*--------------------------------------------------------------------------*/

// register LukFiBlock to the Block factory
SMSpp_insert_in_factory_cpp_1( LukFiBlock );

/*--------------------------------------------------------------------------*/
/*-------------------------- PROTECTED METHODS -----------------------------*/
/*--------------------------------------------------------------------------*/

void LukFiBlock::print( std::ostream &output ) const
{
 output << "LukFiBlock with " << x.size() << " vars " << std::endl;
 }

/*--------------------------------------------------------------------------*/

void LukFiBlock::load( std::istream &input )
{
 if( x.size() )
  throw( std::logic_error( "loading a non-empty LukFiBlock" ) );

 NameF = read_int( input );
 if( ( NameF < 1 ) || ( NameF > 25 ) )
  throw( std::invalid_argument( "invalid name of the function" ) );

 int n = read_int( input );
 if( n < 0 )
  throw( std::invalid_argument( "invalid number of variables" ) );
 SetDimension( n );

 NrCmp = read_int( input );
  if( NrCmp < 0 )
   throw( std::invalid_argument( "invalid number of components" ) );

 for( unsigned int i = 0 ; i < n ; i++ )
  x[ i ].set_Block( this );

 seed = read_int( input );
 if( seed < 0 )
  throw( std::invalid_argument( "invalid seed" ) );

 LukFiFunction::v_col_var vars( x.size() );
 for( int i = 0 ; i < x.size() ; ++i )
  vars[ i ] = &x[ i ];

 f.set_function( new LukFiFunction( std::move( vars ) , true ) , eNoMod );
 auto luk_f = dynamic_cast<LukFiFunction *>( f.get_function() );
 if( luk_f == nullptr )
  throw( std::logic_error( "the objective is not a LukFiFunction" ) );

 luk_f->set_par( LukFiFunction::intNrCmp , NrCmp );
 luk_f->set_par( LukFiFunction::intseed , seed );
 luk_f->set_par( LukFiFunction::intNameF , NameF );

 f.set_Block( this );

 // issue the NBModification - - - - - - - - - - - - - - - - - - - - - - - -

 if( anyone_there() )
  add_Modification( std::make_shared<NBModification>( this ) );

 }  // end( SimpleMILPBlock::load )

/*--------------------------------------------------------------------------*/

void LukFiBlock::serialize( netCDF::NcGroup & group ) const
{
 group.putAtt( "type" , "LukFiBlock" );

 // dimensions and variable declarations
 netCDF::NcDim ndim = group.addDim( "num_vars" , x.size() );

 netCDF::NcDim name_f = group.addDim( "name_f" , NameF );
 netCDF::NcDim nr_cmp = group.addDim( "nr_cmp" , NrCmp );
 netCDF::NcDim vl_seed = group.addDim( "seed" , seed );

 }  // end( LukFiBlock::serialize )

/*--------------------------------------------------------------------------*/

void LukFiBlock::deserialize( netCDF::NcGroup & group ) 
{
 netCDF::NcDim n_dim  = group.getDim( "num_vars" );
 size_t n = n_dim.getSize();

 netCDF::NcDim name_f = group.getDim( "name_f" );
 NameF = name_f.getSize();
 SetDimension( n );

 netCDF::NcDim nr_cmp = group.getDim( "nr_cmp" );
 NrCmp = nr_cmp.getSize();

 netCDF::NcDim vl_seed = group.getDim( "seed" );
 seed = vl_seed.getSize();

 x.resize( n );

 // insert variables

 LukFiFunction::v_col_var p( x.size() );
 for( size_t i = 0 ; i < n ; i++ ) {
  x[ i ].set_Block( this );
  p.push_back( &x[ i ] );
  }

 f.set_function( new LukFiFunction( std::move( p ) , true ) , eNoMod );
 auto luk_f = dynamic_cast<LukFiFunction *>( f.get_function() );
 if( luk_f == nullptr )
  throw( std::logic_error( "the objective is not a LukFiFunction" ) );
 f.set_Block( this );

 luk_f->set_par( LukFiFunction::intNameF , NameF );
 luk_f->set_par( LukFiFunction::intNrCmp , NrCmp );
 luk_f->set_par( LukFiFunction::intseed , seed );

 f.set_Block( this );

 }  // end( LukFiBlock::deserialize )

/*--------------------------------------------------------------------------*/
/*-------------------------- PRIVATE METHODS -------------------------------*/
/*--------------------------------------------------------------------------*/

void LukFiBlock::SetDimension( int n )
{

 // set the dimension of the function  - - - - - - - - - - - - - - - - - - - -
 //  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 switch( NameF ) {
  // Rosenbrock  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 1 ):
   n = int (2);
   break;
  // Crescent    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 2 ):
   n = int (2);
   break;
  // CB2 (Charalambous/Bandler)  - - - - - - - - - - - - - - - - - - - - - - -
  case( 3 ):
   n = int (2);
   break;
  // CB3 (Charalambous/Bandler)  - - - - - - - - - - - - - - - - - - - - - - -
  case( 4 ):
   n = int (2);
   break;
  // DEM (Demyanov/Malozemov)  - - - - - - - - - - - - - - - - - - - - - - - -
  case( 5 ):
   n = int (2);
   break;
  // QL    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 6 ):
   n = int (2);
   break;
  // LQ    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 7 ):
   n = int (2);
   break;
  // Mifflin1      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 8 ):
   n = int (2);
   break;
  // Mifflin2      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 9 ):
   n = int (2);
   break;
  // Wolfe   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 10 ):
   n = int (2);
   break;
  // Rosen-Suzuki    - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 11 ):
   n = int (4);
   break;
  // Shor    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 12 ):
   n = int (5);
   break;
  // Maxquad   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 13 ):
   n = int (10);
   break;
  // Maxq    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 14 ):
   n = int (20);
   break;
  // Maxl    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 15 ):
   n = int (20);
   break;
  // TR48    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 16 ):
   n = int( 48 );
   break;
  // Colville1   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 17 ):
   n = int (5);
   break;
  // HS78  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 18 ):
   n = int (5);
   break;
  // El-Attar  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 19 ):
   n = int (6);
   break;
  // Gill  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 20 ):
   n = int (10);
   break;
  // Steiner2  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 21 ):
   n = int (12);
   break;
  // Goffin  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 22 ):
   n = int (50);
   break;
  // MXHILB    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 23 ):
   n = int (50);
   break;
  // L1HILB    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 24 ):
   n = int (50);
   break;
  // Shell Dual  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 25 ):
   n = int (15);
   break;
  // smooth     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 26 ):
   x.resize( n );
   break;
  // AbsVal     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 27 ):
   x.resize( n );
   break;
  // MaxQR    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 28 ):
   x.resize( n );
   break;
  // Lewis   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 29 ):
   n = int( 2 );
   break;
  }  // end( switch )

 // initialize (static) variables and box constraints - - - - - - - - - - - -

 x.resize( n );

 }  // end( LukFiBlock::SetDimension )

/*--------------------------------------------------------------------------*/

void LukFiBlock::SetInitialPoint( void )
{
 switch( NameF ) {
  // Rosenbrock  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 1 ):
   x[ 0 ].set_value( -1.2 );
   x[ 1 ].set_value( 1.0 );
   break;
  // Crescent    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 2 ):
   x[ 0 ].set_value( -1.5 ) ;
   x[ 1 ].set_value( 2.0 ) ;
   break;
  // CB2 (Charalambous/Bandler)  - - - - - - - - - - - - - - - - - - - - - - -
  case( 3 ):
   x[ 0 ].set_value( 1 ) ;
   x[ 1 ].set_value( -0.1 ) ;
   break;
  // CB3 (Charalambous/Bandler)  - - - - - - - - - - - - - - - - - - - - - - -
  case( 4 ):
   x[ 0 ].set_value( 2 );
   x[ 1 ].set_value( 2 ) ;
   break;
  // DEM (Demyanov/Malozemov)  - - - - - - - - - - - - - - - - - - - - - - - -
  case( 5 ):
   x[ 0 ].set_value( 1 ) ;
   x[ 1 ].set_value( 1 ) ;
   break;
  // QL    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 6 ):
   x[ 0 ].set_value( -1 ) ;
   x[ 1 ].set_value( 5) ;
   break;
  // LQ    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 7 ):
   x[ 0 ].set_value( -0.5 ) ;
   x[ 1 ].set_value( -0.5 ) ;
   break;
  // Mifflin1      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 8 ):
   x[ 0 ].set_value( 0.8 ) ;
   x[ 1 ].set_value( 0.6 ) ;
   break;
  // Mifflin2      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 9 ):
   x[ 0 ].set_value( -1) ;
   x[ 1 ].set_value( -1 ) ;
   break;
  // Wolfe   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 10 ):
   x[ 0 ].set_value( 3 );
   x[ 1 ].set_value( 2 );
   break;
  // Rosen   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 11 ):
   for ( int i = 0 ; i < x.size() ; i++ )
	x[ i ].set_value( 0 );
   break;
  // Shor    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 12 ):
   x[ 0 ].set_value( 0 );  x[ 1 ].set_value( 0 );  x[ 2 ].set_value( 0 );
   x[ 3 ].set_value( 0 );  x[ 4 ].set_value( 1 );
   break;
  // Maxquad   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 13 ):
   for( int i = 0 ; i < x.size() ; i++ )
	x[ i ].set_value( 1 );
   break;
  // Maxq    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 14 ):
   for ( int i = 0 ; i < 10 ; i++ )
	x[ i ].set_value( i + 1 );
   for ( int i = 10 ; i < 20 ; i++ )
  	x[ i ].set_value( -i - 1 );
   break;
  // Maxl    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 15 ):
   for ( int i = 0 ; i < 10 ; i++ )
	x[ i ].set_value( i + 1 );
   for( int i = 10 ; i < 20 ; i++ )
 	x[ i ].set_value( -i - 1 );
   break;
  // TR48    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 16 ):
   for( int i = 0 ; i < x.size() ; i++ )
   x[ i ].set_value( 0 ); /*
   x[0]  =  11.19; x[1]  =  127.2; x[2]  = -129.7; x[3]  =  344.5;
   x[4]  = -40.72; x[5]  = -295.3; x[6]  = -202.3; x[7]  = -382.3;
   x[8]  = -217.7; x[9]  = -307.7; x[10] =  178.1; x[11] =  -4.36;
   x[12] = -123.3; x[13] = -265.3; x[14] =  28.28; x[15] =  70.57;
   x[16] = -31.81; x[17] = -222.3; x[18] =  96.19; x[19] = -52.79;
   x[20] = -34.71; x[21] = -59.16; x[22] = -373.7; x[23] = -28.35;
   x[24] = -141.7; x[25] =   2.28; x[26] =  198.5; x[27] = -69.16;
   x[28] = -26.35; x[29] = -88.72; x[30] =  130.8; x[31] = -12.35;
   x[32] =  -30.7; x[33] = -376.3; x[34] =  23.18; x[35] = -400.3;
   x[36] =  197.1; x[37] = -260.3; x[38] =  813.5; x[39] = -191.7;
   x[40] =  31.29; x[41] =  345.5; x[42] =  -7.72; x[43] =  335.5;
   x[44] =  947.5; x[45] =  722.5; x[46] = -300.3; x[47] =   73.2; */
   break;
  // Colville1   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 17 ):
   x[ 0 ].set_value( 0 );  x[ 1 ].set_value( 0 );  x[ 2 ].set_value( 0 );
   x[ 3 ].set_value( 0 );  x[ 4 ].set_value( 1 );
   break;
  // HS78    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 18 ):
   x[ 0 ].set_value( -2 );  x[ 1 ].set_value( 1.5 );  x[ 2 ].set_value( 2 );
   x[ 3 ].set_value( -1 );  x[ 4 ].set_value( -1 );
   break;
  // El-Attar    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 19 ):
   x[ 0 ].set_value( 2 );  x[ 1 ].set_value( 2 );  x[ 2 ].set_value( 7 );
   x[ 4 ].set_value( 0 );  x[ 5 ].set_value( -2 ); x[ 6 ].set_value( 1 );
   break;
  // Gill  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 20 ):
   for ( int i = 0 ; i < x.size() ; i++ )
    x[ i ].set_value( -0.1 );
   break;
  // Steiner2  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 21 ):
   for ( int i = 0 ; i < x.size() ; i++ )
  	x[ i ].set_value( 0 );
   break;
  // Goffin  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 22 ):
   for ( int i = 0 ; i < x.size() ; i++ )
	x[ i ].set_value( ( i+1 ) - 25.5 );
   break;
  // MXHILB    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 23 ):
   for ( int i = 0 ; i < x.size() ; i++ )
	x[ i ].set_value( 1 );
   break;
  // L1HILB    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 24 ):
   for ( int i = 0 ; i < x.size() ; i++ )
	x[ i ].set_value( 1 );
   break;
  // Shell Dual  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 25 ):
   for ( int i = 0 ; i < x.size() ; i++ )
	if( i != 6 )
	 x[ i ].set_value( 1e-4 );
	else
     x[ i ].set_value( 60 );
   break;
  // smooth    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 26 ):
   for( int i = 0 ; i < x.size() ; i++ )
	x[ i ].set_value( 1.0 );
   break;
  // AbsVal    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 27 ):
   for( int i = 0 ; i < x.size() ; i++ )
   	x[ i ].set_value( -1.0 );
   break;
  // MaxQR    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 28 ):
   for( int i = 0 ; i < x.size() ; i++ )
    x[ i ].set_value( 1.0 );
   break;
  // Lewis  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  case( 29 ):
   x[ 0 ].set_value( -10 ); x[ 1 ].set_value( -10 );
   break;
  }  // end( switch )
 }  // end( LukFiBlock::SetInitialPoint )

/*--------------------------------------------------------------------------*/
/*--------------------- End File LukFiBlock.cpp ----------------------------*/
/*--------------------------------------------------------------------------*/
