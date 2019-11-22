/*--------------------------------------------------------------------------*/
/*----------------------------- File main.cpp ------------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Small main() for testing SimpleMILPBlock. It just creates one and loads it
 * from a stream; little more than a compilation check.
 *
 * \version 0.10
 *
 * \date 23 - 07 - 2018
 *
 * \author Antonio Frangioni \n
 *         Operations Research Group \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * Copyright &copy by Antonio Frangioni
 */
/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>

#include "LukFiBlock.h"
#include "BundleSolver.h"

/*--------------------------------------------------------------------------*/
/*-------------------------------- USING -----------------------------------*/
/*--------------------------------------------------------------------------*/

using namespace std;
using namespace SMSpp_di_unipi_it;

/*--------------------------------------------------------------------------*/
/*----------------------------- CONSTANTS ----------------------------------*/
/*--------------------------------------------------------------------------*/

const char *const logF = "log.bn";

/*--------------------------------------------------------------------------*/
/*--------------------------------- Main -----------------------------------*/
/*--------------------------------------------------------------------------*/

int main( int argc , char **argv )
{
 if( argc <1 || argc>2) {
  cerr << "Usage: " << argv[ 0 ] << " LukFi_file_name NC4_file_name [NC4_file_name_2]" << endl;
  return( 1 );
  }

 ifstream ProbFile( argv[ 1 ] );
 if( ! ProbFile.is_open() ) {
  cerr << "Error: cannot open file " << argv[ 1 ] << endl;
  return( 1 );
  }

 Block *sLukFi = Block::new_Block( "LukFiBlock" );
 ProbFile >> *sLukFi;
 cout << *sLukFi;

 BlockSolverConfig * bsc = new BlockSolverConfig;
 ProbFile >> *( bsc );

 sLukFi->set_SolverConfig( bsc );
 delete bsc;

 Solver * slvr = (sLukFi->get_registered_solvers()).front();

 // open log-file - - - - - - - - - - -  - - - - - - - - - - - - - - - - - -
 //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 ofstream LOGFile( logF , ofstream::out );
 if( ! LOGFile.is_open() )
  cerr << "Warning: cannot open log file """ << logF << """" << endl;
 else
  slvr->set_log( &LOGFile );

 int rtrn = slvr->compute( false );

 LOGFile << std::endl << std::endl << "f* = "
		 << slvr->get_lb() << " (optimal value)" << std::endl;

int NumVar = 0;  // count the number of Variable
 auto v_s_Variable = sLukFi->get_static_variables();
 for( auto & el : v_s_Variable ) {
  if( un_any_thing_0( ColVariable , el , ++NumVar ) )
   break;
  if( un_any_thing_1( ColVariable , el , NumVar += var.size() ) )
   break;
  if( un_any_thing_K( ColVariable , el , NumVar += var.size() ) )
   break;
  throw( std::logic_error( "some static Variable is not a ColVariable" ) );
  }

 std::vector<ColVariable *> x( NumVar );
 Index count = 0;
 for( auto & el : v_s_Variable )
  un_any_static( el , [ & ]( ColVariable & static_var ) {
                       x[ count++ ] = & static_var;
                       } ,
		  un_any_type<ColVariable>() );

 count = 0;
 LOGFile << std::endl << "x* = "<< std::endl;
 for( auto & var : x )
 LOGFile  << "[" << count++ << "] "
		  << var->get_value() << endl;

 delete sLukFi;

 return( 0 );
 }

/*--------------------------------------------------------------------------*/
/*------------------------- End File main.cpp ------------------------------*/
/*--------------------------------------------------------------------------*/

