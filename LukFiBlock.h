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
#include "ColVariable.h"
#include "FRowConstraint.h"
#include "FRealObjective.h"
#include "OneVarConstraint.h"
#include "Configuration.h"

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
/*------------------------- CLASS LukFiFunction -----------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------- GENERAL NOTES --------------------------------*/
/*--------------------------------------------------------------------------*/

class LukFiFunction : public C05Function {

/*--------------------------------------------------------------------------*/
/*----------------------- PUBLIC PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

 public:

/*--------------------------------------------------------------------------*/
/*---------------------- PUBLIC TYPES OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Public Types
 *  @{ */

 typedef std::vector< ColVariable * > v_col_var; ///< a vector of dual_pair

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

 typedef const std::vector< ColVariable * > c_v_col_var; ///< a const vector of dual_pair

/*--------------------------------------------------------------------------*/

 typedef std::vector<double> dblVR1;
 typedef std::vector<dblVR1> dblVR2;
 typedef std::vector<dblVR2> dblVR3;

/*--------------------------------------------------------------------------*/
  /// virtualized concrete iterator
  /** A concrete class deriving from ThinVarDepInterface::v_iterator and
   * implementing the concrete iterator for "sifting through" the "active"
   * Variable of a LagBFunction. */

 class v_iterator : public ThinVarDepInterface::v_iterator
  {
   public:

   v_iterator( v_col_var::iterator itr ) : itr_( itr ) { }
   virtual v_iterator * clone( void ) override {
    return( new v_iterator( itr_ ) );
    }

   virtual void operator++( void ) override final { (itr_)++; }
   virtual reference operator*( void ) const override final {
    return( *(*itr_) );
    }
   virtual pointer operator->( void ) const override final {
    return( *itr_ );
    }
   virtual bool operator==( const ThinVarDepInterface::v_iterator & rhs )
    const override final {
    #ifdef NDEBUG
     auto tmp = static_cast<const LukFiFunction::v_iterator *>( & rhs );
     return( itr_ == tmp->itr_ );
    #else
     auto tmp = dynamic_cast<const LukFiFunction::v_iterator *>( & rhs );
     return( tmp ? itr_ == tmp->itr_ : false );
    #endif
    }
   virtual bool operator!=( const ThinVarDepInterface::v_iterator & rhs )
    const override final {
    #ifdef NDEBUG
     auto tmp = static_cast<const LukFiFunction::v_iterator *>( & rhs );
     return( itr_ != tmp->itr_ );
    #else
     auto tmp = dynamic_cast<const LukFiFunction::v_iterator *>( & rhs );
     return( tmp ? itr_ != tmp->itr_ : false );
    #endif
    }

   private:

   v_col_var::iterator itr_;
   };

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /// virtualized concrete const_iterator
  /** A concrete class deriving from ThinVarDepInterface::v_const_iterator and
    * implementing the concrete iterator for sifting through the "active"
    * Variable of a LinearFunction. */

  class v_const_iterator : public ThinVarDepInterface::v_const_iterator
  {
   public:

   v_const_iterator( v_col_var::const_iterator itr ) : itr_( itr ) { }
   virtual v_const_iterator * clone( void ) override {
    return( new v_const_iterator( itr_ ) );
    }

   virtual void operator++( void ) override final { (itr_)++; }
   virtual reference operator*( void ) const override final {
    return( *(*itr_) );
    }
   virtual pointer operator->( void ) const override final {
    return( *itr_ );
    }
   virtual bool operator==( const ThinVarDepInterface::v_const_iterator & rhs )
    const override final {
    #ifdef NDEBUG
     auto tmp = static_cast<const LukFiFunction::v_const_iterator *>( & rhs );
     return( itr_ == tmp->itr_ );
    #else
     auto tmp = dynamic_cast<const LukFiFunction::v_const_iterator *>( & rhs
  								       );
     return( tmp ? itr_ == tmp->itr_ : false );
    #endif
    }
   virtual bool operator!=( const ThinVarDepInterface::v_const_iterator & rhs )
    const override final {
    #ifdef NDEBUG
     auto tmp = static_cast<const LukFiFunction::v_const_iterator *>( & rhs );
     return( itr_ != tmp->itr_ );
    #else
     auto tmp = dynamic_cast<const LukFiFunction::v_const_iterator *>( & rhs );
     return( tmp ? itr_ != tmp->itr_ : false );
    #endif
    }

   private:

   v_col_var::const_iterator itr_;
   };

/*--------------------------------------------------------------------------*/

  enum int_par_type_LukF {
   intNrCmp = intLastParC0F , // number of component functions
   intseed ,                  // seed for random number generation
   intLastParLukF
   };  // end( int_par_type_LukF )

/**@} ----------------------------------------------------------------------*/
/*--------------------- PUBLIC METHODS OF THE CLASS ------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------- CONSTRUCTOR AND DESTRUCTOR -------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Constructor and Destructor
 *  @{ */

/*--------------------------------------------------------------------------*/
  /// constructor of LukFiFunction:
  /** constructor of LukFiFunction. It accepts the name of the function
      and the pointer to the vector of variables. */

 LukFiFunction( int name , v_col_var && vars ,
		 const bool ordered = false );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 /// destructor of LukFiFunction: delete the allocated memory.
 /** destructor of LukFiFunction. It deletes delete the subgradient. */

 virtual ~LukFiFunction( void ) { SubG.clear(); };

/*--------------------------------------------------------------------------*/

 virtual void clear( void ) override { };

/**@} ----------------------------------------------------------------------*/
/*-------------------------- OTHER INITIALIZATIONS -------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Other initializations
 *  @{ */

 /// set a given integer (int) numerical parameter
 /** Set a given integer (int) numerical parameter. The method sets the maximum
  *  size of both the local and the global pool  */

 virtual void set_par( const idx_type par , const int value ) override;

/*--------------------------------------------------------------------------*/
 /** As stated above, the Observer of a LukFiFunction is assumed to be a
  * FRealObjective. */

// ?? virtual void register_Observer( Observer * const observer = nullptr ) override {}

/**@} ----------------------------------------------------------------------*/
/*-------------------- Methods for handling Modification -------------------*/
/*--------------------------------------------------------------------------*/
/** @name Methods for handling Modification
 *  @{ */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 /// remove a bunch of *dynamic* Lagrangian pairs <y, g(x)>,
 /** This method removes a bunch of Lagrangian pairs. The structure LagMatrix
  *  used to compute the Lagrangian costs needs to be update.  */

 virtual void remove_variables( std::vector<Variable *> && vars ,
				const bool ordered = false ,
				c_ModParam issueMod = eModBlck ) override {

  throw( std::logic_error( "variables remotion is not allowed" ) );
  }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 virtual void remove_variable( Variable * var ,
 			       c_ModParam issueMod = eModBlck ) override {
  throw( std::logic_error( "variables remotion is not allowed" ) );
  }

/**@} ----------------------------------------------------------------------*/
/*--------- METHODS DESCRIBING THE BEHAVIOR OF THE LukFiFunction -----------*/
/*--------------------------------------------------------------------------*/
/** @name Methods describing the behavior of the LukFiFunction
 *  @{ */

 /// compute the Function

 virtual int compute( bool changedvars = true ) override;

/*--------------------------------------------------------------------------*/
 /// returns the value of the Function{}

 virtual FunctionValue get_value( void ) const override {
  return( FiVal );
  }

/*--------------------------------------------------------------------------*/
/// retrieve the coefficients (g vector) of a linearization in a vector
/** This method retrieves the vector of coefficients g that is the (largest)
  * part of the linearization with the given name.
  *
  * This implements the virtual function of class C05Function.  */

 virtual void get_linearization_coefficients( FunctionValue * g ,
   const LinearizationName name = Inf<LinearizationName>() ,
   c_Vec_Index & indices = {} , c_Index start = 0 ,
   c_Index end = Inf<Index>() ) override final;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

/// retrieve the coefficients (g) of a linearization in a sparse vector
/** This method retrieves the sparse vector of coefficients g that is part
  * of a linearization.
  *
  * This implements the virtual function of class C05Function. */

 virtual void get_linearization_coefficients( SparseVector &g ,
   const LinearizationName name = Inf<LinearizationName>() ,
   c_Vec_Index & indices = {} , c_Index start = 0 ,
   c_Index end = Inf<Index>() ) override final {

  throw( std::logic_error( "sparsify is not allowed" ) );
  }

/*--------------------------------------------------------------------------*/

 virtual FunctionValue get_linearization_constant(
		 const LinearizationName name = Inf<Index>() ) override final;

/**@} ----------------------------------------------------------------------*/
/*------------------- METHODS FOR HANDLING THE PARAMETERS ------------------*/
/*--------------------------------------------------------------------------*/
/** @name Handling the parameters of the LukFiFunction
 *  @{ */

 virtual idx_type get_num_int_par( void ) const override
 {
  return( intLastParLukF );
  }

/*--------------------------------------------------------------------------*/

 virtual int get_int_par( const idx_type par ) const override;

/*--------------------------------------------------------------------------*/

 virtual idx_type int_par_str2idx( const std::string & name ) const override;

/*--------------------------------------------------------------------------*/

 virtual const std::string & int_par_idx2str( const idx_type idx )
   const override;

/*--------------------------------------------------------------------------*/

 virtual int get_dflt_int_par( const idx_type par ) const override;

/**@} ----------------------------------------------------------------------*/
/*----- METHODS FOR HANDLING "ACTIVE" Variable IN THE LukFiFunction ---------*/
/*--------------------------------------------------------------------------*/
/** @name Methods for handling the set of "active" Variable in the
 * LukFiFunction; this is the actual concrete implementation exploiting the
 * vector lag_p of Lagrangian pairs.
 * @{ */

 virtual Index get_num_active_var( void ) const override final {
  return( v_vars.size() );
  }

/*--------------------------------------------------------------------------*/

 virtual Index is_active( const Variable * const var ) const override final;

/*--------------------------------------------------------------------------*/

 virtual void map_active( c_Vec_p_Var & vars , Vec_Index & map ,
			  const bool ordered = false ) const override final;

/*--------------------------------------------------------------------------*/

 virtual Variable *get_active_var( const Index i ) const override final {
  return( *( v_vars.begin() + i ) );
  }
/*--------------------------------------------------------------------------*/

 virtual v_iterator * v_begin( void ) override final {
  return( new LukFiFunction::v_iterator( v_vars.begin() ) );
  }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 virtual v_const_iterator * v_begin( void ) const override final {
  return( new LukFiFunction::v_const_iterator( v_vars.begin() )  );
  }

/*--------------------------------------------------------------------------*/

 virtual v_iterator * v_end( void ) override final {
  return( new LukFiFunction::v_iterator( v_vars.end() ) );
  }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 virtual v_const_iterator * v_end( void ) const override final {
  return( new LukFiFunction::v_const_iterator( v_vars.end() ) );
  }

/**@} ----------------------------------------------------------------------*/
/*-------------------- PROTECTED PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

 protected:

/*--------------------------------------------------------------------------*/
/*--------------------------- PROTECTED METHODS ----------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Protected methods for inserting and extracting
 *  @{ */

 /// printing the LukFiFunction
 virtual void print( std::ostream &output ) const override {}

/**@} ----------------------------------------------------------------------*/
/*--------------------------- PROTECTED FIELDS  ----------------------------*/
/*--------------------------------------------------------------------------*/

 v_col_var v_vars;
 int NameF;       ///< name (number) of the function at point Lambda
 int NrCmp;       ///< number of component functions
 int seed;        ///< seed for random number generation

 dblVR1 bQR;
 dblVR1 aQR;
 dblVR2 cQR;

 Index GPMaxSz;

 std::vector<FunctionValue> SubG;
 double FiVal;

/*--------------------------------------------------------------------------*/

 const static std::vector<int> dflt_int_par;
 ///< the (static const) vector of int parameters default values

 const static std::vector< std::string > int_pars_str;
 ///< the (static const) vector of int parameters names

 const static std::map< std::string , idx_type > int_pars_map;
  ///< the (static const) map for int parameters names

/**@} ----------------------------------------------------------------------*/
/*--------------------- PRIVATE PART OF THE CLASS --------------------------*/
/*--------------------------------------------------------------------------*/

 private:

/*--------------------------------------------------------------------------*/
/*-------------------------- PRIVATE METHODS -------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*---------------------------- PRIVATE FIELDS ------------------------------*/
/*--------------------------------------------------------------------------*/

 };  // end( class( LukFiFunction ) )

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
