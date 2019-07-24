/*--------------------------------------------------------------------------*/
/*--------------------------- File LukFiBlock.h ----------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Header file for the *concrete* class LukFiBlock, which implements the
 * Block concept [see Block.h] for a very simple "flat" problem having
 * a nonsmooth objective function and no constraints.
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
/*----------------------------- DEFINITIONS --------------------------------*/
/*--------------------------------------------------------------------------*/

#ifndef __LukFiBlock
 #define __LukFiBlock
                      /* self-identification: #endif at the end of the file */

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include "Block.h"
#include "C05Function.h"
#include "LukFiFunction.h"
#include "FRowConstraint.h"
#include "FRealObjective.h"
#include "OneVarConstraint.h"

/*--------------------------------------------------------------------------*/
/*--------------------------- NAMESPACE ------------------------------------*/
/*--------------------------------------------------------------------------*/

/// namespace for the Structured Modeling System++ (SMS++)
namespace SMSpp_di_unipi_it
{
/*--------------------------------------------------------------------------*/
/*-------------------- SimpleMILPBlock-RELATED TYPES -----------------------*/
/*--------------------------------------------------------------------------*/
/** @defgroup LukFiBlock_TYPES SimpleMILPBlock-related types
 *  @{ */

/** @}  end( group( LukFiBlock_TYPES ) ) */
/*--------------------------------------------------------------------------*/
/*------------------------------- CLASSES ----------------------------------*/
/*--------------------------------------------------------------------------*/
/** @defgroup SimpleMILPBlock_CLASSES Classes in SimpleMILPBlock.h
 *  @{ */

/*--------------------------------------------------------------------------*/
/*-------------------------- CLASS LukFiBlock ------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------- GENERAL NOTES --------------------------------*/
/*--------------------------------------------------------------------------*/
/// implementation of a simple LukFi Block concept

class LukFiBlock : public Block {

/*--------------------------------------------------------------------------*/
/*----------------------- PUBLIC PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

public:

/*--------------------------------------------------------------------------*/
/*---------------------------- PUBLIC TYPES --------------------------------*/
/*--------------------------------------------------------------------------*/

 typedef std::vector<double> DblRow;
 typedef std::vector<DblRow> DblMat;

/*--------------------------------------------------------------------------*/
/*--------------------- PUBLIC METHODS OF THE CLASS ------------------------*/
/*--------------------------------------------------------------------------*/
/*---------------------------- CONSTRUCTOR ---------------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Constructor and Destructor
 *  @{ */

 /// constructor of LukFiBlock
 /** Constructor of LukFiBlock. It accepts a pointer to the father
  * Block, which can be of any type. */

 LukFiBlock( Block *father = nullptr ) : Block( father ) {}

/*--------------------------------------------------------------------------*/
 /// destructor of LukFiBlock
 /** Destructor of LukFiBlock. . */

 virtual ~LukFiBlock() {
  f.clear();
  }

/*@} -----------------------------------------------------------------------*/
/*-------------------------- OTHER INITIALIZATIONS -------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Other initializations
 *  @{ */

/*@} -----------------------------------------------------------------------*/
/*-------------------------------- ACCESSORS -------------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Accessors
 *  @{ */

 std::vector<ColVariable> & get_x( void ) { return( x ); }

/*@}------------------------------------------------------------------------*/
/*-------------------- PROTECTED PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

 protected:

/*--------------------------------------------------------------------------*/
/*-------------------------- PROTECTED METHODS -----------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Protected methods for inserting and extracting
    @{ */

 virtual void print( std::ostream &output ) const override;
 ///< print the LukFiBlock on an ostream with the given verbosity

/*--------------------------------------------------------------------------*/

 virtual void load( std::istream &input ) override;

 ///< load the LukFiBlock out of an istream
 /**< Load the LukFiBlock out of an istream. The format is:
  *
  * number of variables ,
  * name of the function
  * number of components ,
  * seed
  */

 virtual void serialize( netCDF::NcGroup & file ) const override;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 /// extends Block::deserialize( netCDF::NcGroup )
 /** Extends Block::deserialize( netCDF::NcGroup ) to the specific format of
  * a LukFiBlock.  */

 virtual void deserialize( netCDF::NcGroup & group ) override;

/*@}------------------------------------------------------------------------*/
/*--------------------------- PROTECTED FIELDS  ----------------------------*/
/*--------------------------------------------------------------------------*/

 std::vector<ColVariable> x;       /// the variables

 FRealObjective f;                 /// the objective function


/*--------------------------------------------------------------------------*/
/*--------------------- PRIVATE PART OF THE CLASS --------------------------*/
/*--------------------------------------------------------------------------*/

 private:

/*--------------------------------------------------------------------------*/
/*-------------------------- PRIVATE METHODS -------------------------------*/
/*--------------------------------------------------------------------------*/

 void SetDimension( int n );
 void SetInitialPoint( void );

/*--------------------------------------------------------------------------*/
/*---------------------------- PRIVATE FIELDS ------------------------------*/
/*--------------------------------------------------------------------------*/

 int NameF;       ///< name (number) of the function at point Lambda
 int NrCmp;       ///< number of component functions
 int seed;        ///< seed for random number generation

 SMSpp_insert_in_factory_h;        // insert it in the Block factory

/*--------------------------------------------------------------------------*/

};  // end( class( LukFiBlock ) )

/*--------------------------------------------------------------------------*/

/*@}  end( group( LukFiBlock_CLASSES ) ) -----------------------------------*/
/*--------------------------------------------------------------------------*/

 }  // end( namespace SMSpp_di_unipi_it )

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

#endif  /* LukFiBlock.h included */

/*--------------------------------------------------------------------------*/
/*--------------------- End File LukFiBlock.h ------------------------------*/
/*--------------------------------------------------------------------------*/
