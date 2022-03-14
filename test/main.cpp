/*--------------------------------------------------------------------------*/
/*----------------------------- File main.cpp ------------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Small main() for testing SimpleMILPBlock. It just creates one and loads it
 * from a stream; little more than a compilation check.
 *
 * \author Antonio Frangioni \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \author Enrico Gorgone \n
 *         Dipartimento di Matematica ed Informatica \n
 *         Universita' di Cagliari \n
 *
 * Copyright &copy by Antonio Frangioni
 */
/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>

//#include "LukFiBlock.h"

#include "BlockSolverConfig.h"

//#include "BundleSolver.h"

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
  cerr << "Usage: " << argv[ 0 ]
       << " LukFi_file_name NC4_file_name [NC4_file_name_2]" << endl;
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

 bsc->apply( sLukFi );
 bsc->clear();

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

 bsc->apply( sLukFi );

 delete bsc;

 delete sLukFi;

 return( 0 );
 }

/*--------------------------------------------------------------------------*/
/*------------------------- End File main.cpp ------------------------------*/
/*--------------------------------------------------------------------------*/

