/*--------------------------------------------------------------------------*/
/*------------------------ File LukFiFunction.h ---------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Header file for the class LukFiFunction, which implements the Luksan
 * functions and other non-smooth functions.
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
 * Copyright &copy; by Antonio Frangioni, Enrico Gorgone.
 */
/*--------------------------------------------------------------------------*/
/*----------------------------- DEFINITIONS --------------------------------*/
/*--------------------------------------------------------------------------*/

#ifndef __LukFiFunction
 #define __LukFiFunction
                      /* self-identification: #endif at the end of the file */

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include "C05Function.h"
#include "Block.h"
#include "ColVariable.h"
#include "FRealObjective.h"
#include "Configuration.h"

/*--------------------------------------------------------------------------*/
/*--------------------------- NAMESPACE ------------------------------------*/
/*--------------------------------------------------------------------------*/

/// namespace for the Structured Modeling System++ (SMS++)
namespace SMSpp_di_unipi_it
{

/*--------------------------------------------------------------------------*/
/*------------------------------- CLASSES ----------------------------------*/
/*--------------------------------------------------------------------------*/
/** @defgroup LagFun_CLASSES Classes in LukFiFunction.h
 *  @{ */

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
   intNameF = intLastParC0F , // name (number) of the function
   intNrCmp ,                 // number of component functions
   intseed ,                   // seed for random number generation
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
  /// constructor of LukFiFunction: does nothing

 LukFiFunction( v_col_var && vars , const bool ordered = false );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 /// destructor of LukFiFunction: delete the allocated memory.
 /** destructor of LukFiFunction. It deletes delete the global pool and the
     LagMatrix which is used to change the Lagrangian costs. */

 virtual ~LukFiFunction( void ) { };

/*--------------------------------------------------------------------------*/

 virtual void clear( void ) override { };

/**@} ----------------------------------------------------------------------*/
/*-------------------------- OTHER INITIALIZATIONS -------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Other initializations
 *  @{ */

  /// set the whole (empty) set of parameters in one blow

  virtual void set_ComputeConfig( ComputeConfig *scfg = nullptr )
   override final;

/*--------------------------------------------------------------------------*/

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

  throw( std::logic_error( "sparsify is not allowed" ) ); // ????
  }

/*--------------------------------------------------------------------------*/

 virtual FunctionValue get_linearization_constant(
		 const LinearizationName name = Inf<Index>() ) override final {

  return( 0 ); // ????
  }

/**@} ----------------------------------------------------------------------*/
/*------------------- METHODS FOR HANDLING THE PARAMETERS ------------------*/
/*--------------------------------------------------------------------------*/
/** @name Handling the parameters of the LukFiFunction
 *  @{ */

 ///< get the whole (empty) set of parameters in one blow
 /** Although a LukFiFunction formally has a lot of parameters, in fact it
  * "listens to no-one"; hence, the implementation of get_ComputeConfig() is
  * quite a trivial one.
  *
  * ComputeConfig is assumed to be of the SimpleConfig_p_p type wherein
  * the field f_value is a Configuration pointers pair. The first element
  * of that pair is a BlockSolverConfig and the second one is a
  * BlockConfig. */

 virtual ComputeConfig * get_ComputeConfig( bool all = false ,
					    ComputeConfig * ocfg = nullptr )
  const override final;

/*--------------------------------------------------------------------------*/

 virtual int get_int_par( const idx_type par ) const override;

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

/** @} end( group( LagFun_CLASSES ) ) --------------------------------------*/
/*--------------------------------------------------------------------------*/

 }  // end( namespace SMSpp_di_unipi_it )

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

#endif  /* LukFiFunction.h included */

/*--------------------------------------------------------------------------*/
/*--------------------- End File LukFiFunction.h ----------------------------*/
/*--------------------------------------------------------------------------*/
