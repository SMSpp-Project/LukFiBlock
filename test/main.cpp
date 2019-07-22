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

/*--------------------------------------------------------------------------*/
/*-------------------------------- USING -----------------------------------*/
/*--------------------------------------------------------------------------*/

using namespace std;
using namespace SMSpp_di_unipi_it;

/*--------------------------------------------------------------------------*/
/*--------------------------------- Main -----------------------------------*/
/*--------------------------------------------------------------------------*/

int main( int argc , char **argv )
{
 if( argc <3 || argc>4) {
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

 sLukFi->serialize( argv[ 2 ] );

 cout << "Serialized " << argv[ 1 ] << " to " << argv[ 2 ] <<endl;
 
 if( argc == 4 ) {
  Block *P1 = Block::deserialize( argv[ 2 ] );

  cout << "De-Serialized " << argv[ 2 ] << endl;

  P1->serialize( argv[ 3 ] );

  cout << "Re-Serialized " << argv[ 2 ] << " to " << argv[ 3 ] <<endl;

  delete P1;
  }

 delete sLukFi;

 return( 0 );
 }

/*--------------------------------------------------------------------------*/
/*------------------------- End File main.cpp ------------------------------*/
/*--------------------------------------------------------------------------*/

