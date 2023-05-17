/*--------------------------------------------------------------------------*/
/*--------------------------- File LukFiBlock.h ----------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Header file for the *concrete* class LukFiBlock, which implements the
 * Block concept [see Block.h] for a very simple "flat" problem having
 * a nonsmooth objective function and no constraints.
 *
 * \author Antonio Frangioni \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \author Enrico Gorgone \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \copyright &copy; by Antonio Frangioni
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
//#include "ColVariable.h"
#include "FRowConstraint.h"
#include "FRealObjective.h"
#include "OneVarConstraint.h"
//#include "Configuration.h"

#include <random>

/*--------------------------------------------------------------------------*/
/*--------------------------- NAMESPACE ------------------------------------*/
/*--------------------------------------------------------------------------*/

/// namespace for the Structured Modeling System++ (SMS++)
namespace SMSpp_di_unipi_it
{
/*--------------------------------------------------------------------------*/
/*-------------------------- CLASS LukFiBlock ------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------- GENERAL NOTES --------------------------------*/
/*--------------------------------------------------------------------------*/
 /// Implementation of a simple LukFi Block concept.
 /* In the sequel is reported the optimal value of the implemented
    functions. For a detailed description we refer to the paper
    "Piecewise-quadratic approximations in convex numerical optimization"
     Siam Journal on Optimization. 21(4):1418-1438
    by A. Astorino, F. Frangioni, M. Gaudioso and E. Gorgone

  1. Rosenbrock  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - nonconvex
     opt. value = 0

  2. Crescent    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - nonconvex
     opt. value = 0

  3. CB2 (Charalambous/Bandler)  - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 1.9522245

  4. CB3 (Charalambous/Bandler)  - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 2

  5. DEM (Demyanov/Malozemov)  - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = -3

  6. QL    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 7.20

  7. LQ    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = -1.4142136

  8. Mifflin1      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = -1

  9. Mifflin2      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - nonconvex
     opt. value = -1

  10. Wolfe   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = -8

  11. Rosen-Suzuki    - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = -44

  12. Shor    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 22.600162

  13. Maxquad   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = -0.8414083

  14. Maxq    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0

  15. Maxl    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0

  16. TR48    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = - 638565.0

  17. Colville1   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - nonconvex
     opt. value = -32.348679

  18. HS78  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - nonconvex
     opt. value = -2.9197004

  19. Gill  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - nonconvex
     opt. value = 9.7857

  20. Goffin  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0

  21. MXHILB    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0

  22. L1HILB    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0

  23. smooth     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0

  24. AbsVal     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0

  25. MaxQR    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = not defined

  26. Lewis   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     - convex
     opt. value = 0       */

class LukFiBlock : public Block {

/*--------------------------------------------------------------------------*/
/*----------------------- PUBLIC PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

public:

/*--------------------------------------------------------------------------*/
/*---------------------------- PUBLIC TYPES --------------------------------*/
/*--------------------------------------------------------------------------*/

 typedef std::vector< double > DblRow;

 typedef std::vector< DblRow > DblMat;

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

 typedef const std::vector< ColVariable * > c_v_col_var;
 ///< a const vector of dual_pair

/*--------------------------------------------------------------------------*/

 typedef std::vector< double > dblVR1;

 typedef std::vector< dblVR1 > dblVR2;

 typedef std::vector< dblVR2 > dblVR3;

/*--------------------------------------------------------------------------*/
 /// virtualized concrete iterator
 /** A concrete class deriving from ThinVarDepInterface::v_iterator and
  * implementing the concrete iterator for "sifting through" the "active"
  * Variable of a LukFiFunction. */

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
     auto tmp = static_cast< const LukFiFunction::v_iterator * >( & rhs );
     return( itr_ == tmp->itr_ );
    #else
     auto tmp = dynamic_cast< const LukFiFunction::v_iterator * >( & rhs );
     return( tmp ? itr_ == tmp->itr_ : false );
    #endif
    }
   virtual bool operator!=( const ThinVarDepInterface::v_iterator & rhs )
    const override final {
    #ifdef NDEBUG
     auto tmp = static_cast< const LukFiFunction::v_iterator * >( & rhs );
     return( itr_ != tmp->itr_ );
    #else
     auto tmp = dynamic_cast< const LukFiFunction::v_iterator * >( & rhs );
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
  * Variable of a LukFiFunction. */

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
     auto tmp = static_cast< const LukFiFunction::v_const_iterator * >( & rhs );
     return( itr_ == tmp->itr_ );
    #else
     auto tmp = dynamic_cast< const LukFiFunction::v_const_iterator * >( & rhs
  								       );
     return( tmp ? itr_ == tmp->itr_ : false );
    #endif
    }
   virtual bool operator!=( const ThinVarDepInterface::v_const_iterator & rhs )
    const override final {
    #ifdef NDEBUG
     auto tmp = static_cast< const LukFiFunction::v_const_iterator * >( & rhs );
     return( itr_ != tmp->itr_ );
    #else
     auto tmp = dynamic_cast< const LukFiFunction::v_const_iterator * >( & rhs );
     return( tmp ? itr_ != tmp->itr_ : false );
    #endif
    }

   private:

   v_col_var::const_iterator itr_;
   };

/*--------------------------------------------------------------------------*/

  enum int_par_type_LukF {
   intNrCmp = intLastParC05F , // number of component functions
   intseed ,                  // seed for random number generation
   intLastParLukF
   };  // end( int_par_type_LukF )

/** @} ---------------------------------------------------------------------*/
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

 LukFiFunction( int name , v_col_var && vars );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 /// destructor of LukFiFunction: delete the allocated memory.
 /** destructor of LukFiFunction. It deletes delete the subgradient. */

 virtual ~LukFiFunction( void ) { SubG.clear(); };

/*--------------------------------------------------------------------------*/

 void clear( void ) override { };

/**@} ----------------------------------------------------------------------*/
/*-------------------------- OTHER INITIALIZATIONS -------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Other initializations
 *  @{ */

 /// set a given integer (int) numerical parameter
 /** Set a given integer (int) numerical parameter. The method sets the maximum
  *  size of both the local and the global pool  */

 void set_par( const idx_type par , const int value ) override;

/*--------------------------------------------------------------------------*/
 /** As stated above, the Observer of a LukFiFunction is assumed to be a
  * FRealObjective. */

// ?? void register_Observer( Observer * const observer = nullptr ) override {}

/**@} ----------------------------------------------------------------------*/
/*-------------------- Methods for handling Modification -------------------*/
/*--------------------------------------------------------------------------*/
/** @name Methods for handling Modification
 *  @{ */

 void remove_variable( Index i , c_ModParam issueMod = eModBlck ) override {
  throw( std::logic_error( "variables remotion is not allowed" ) );
  }

/**@} ----------------------------------------------------------------------*/
/*--------- METHODS DESCRIBING THE BEHAVIOR OF THE LukFiFunction -----------*/
/*--------------------------------------------------------------------------*/
/** @name Methods describing the behavior of the LukFiFunction
 *  @{ */

 /// compute the Function

 int compute( bool changedvars = true ) override;

/*--------------------------------------------------------------------------*/
 /// returns the value of the Function{}

 FunctionValue get_value( void ) const override { return( FiVal ); }

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 /// returns true only if this Function is convex

 bool is_convex( void ) const override { return( true ); }

/*--------------------------------------------------------------------------*/
/// retrieve the coefficients (g vector) of a linearization in a vector
/** This method retrieves the vector of coefficients g that is the (largest)
  * part of the linearization with the given name.
  *
  * This implements the virtual function of class C05Function.  */

 void get_linearization_coefficients( FunctionValue * g ,
				      Range range = INFRange ,
				      Index name = Inf< Index >() )
  override final;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

/// retrieve the coefficients (g) of a linearization in a sparse vector
/** This method retrieves the sparse vector of coefficients g that is part
  * of a linearization.
  *
  * This implements the virtual function of class C05Function. */

 void get_linearization_coefficients( FunctionValue * g , c_Subset & subset ,
				      bool ordered = false ,
				      Index name = Inf< Index >() )
  override final;

/*--------------------------------------------------------------------------*/

 FunctionValue get_linearization_constant( Index name = Inf< Index >() )
  override final;

/**@} ----------------------------------------------------------------------*/
/*------------------- METHODS FOR HANDLING THE PARAMETERS ------------------*/
/*--------------------------------------------------------------------------*/
/** @name Handling the parameters of the LukFiFunction
 *  @{ */

 idx_type get_num_int_par( void ) const override {
  return( intLastParLukF );
  }

/*--------------------------------------------------------------------------*/

 int get_int_par( idx_type par ) const override;

/*--------------------------------------------------------------------------*/

 idx_type int_par_str2idx( const std::string & name ) const override;

/*--------------------------------------------------------------------------*/

 const std::string & int_par_idx2str( idx_type idx ) const override;

/*--------------------------------------------------------------------------*/

 int get_dflt_int_par( idx_type par ) const override;

/** @} ---------------------------------------------------------------------*/
/*----- METHODS FOR HANDLING "ACTIVE" Variable IN THE LukFiFunction ---------*/
/*--------------------------------------------------------------------------*/
/** @name Methods for handling the set of "active" Variable in the
 * LukFiFunction; this is the actual concrete implementation exploiting the
 * vector lag_p of Lagrangian pairs.
 * @{ */

 Index get_num_active_var( void ) const override final {
  return( v_vars.size() );
  }

/*--------------------------------------------------------------------------*/

 Index is_active( const Variable * var ) const override final;

/*--------------------------------------------------------------------------*/

 void map_active( c_Vec_p_Var & vars , Subset & map ,
		  bool ordered = false ) const override final;

/*--------------------------------------------------------------------------*/

 Variable * get_active_var( const Index i ) const override final {
  return( *( v_vars.begin() + i ) );
  }

/*--------------------------------------------------------------------------*/

 v_iterator * v_begin( void ) override final {
  return( new LukFiFunction::v_iterator( v_vars.begin() ) );
  }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 v_const_iterator * v_begin( void ) const override final {
  return( new LukFiFunction::v_const_iterator( v_vars.begin() )  );
  }

/*--------------------------------------------------------------------------*/

 v_iterator * v_end( void ) override final {
  return( new LukFiFunction::v_iterator( v_vars.end() ) );
  }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

 v_const_iterator * v_end( void ) const override final {
  return( new LukFiFunction::v_const_iterator( v_vars.end() ) );
  }

/** @} ---------------------------------------------------------------------*/
/*-------------------- PROTECTED PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

 protected:

/*--------------------------------------------------------------------------*/
/*--------------------------- PROTECTED METHODS ----------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Protected methods for inserting and extracting
 *  @{ */

 /// printing the LukFiFunction
 void print( std::ostream & output ) const override {}

/** @} ---------------------------------------------------------------------*/
/*--------------------------- PROTECTED FIELDS  ----------------------------*/
/*--------------------------------------------------------------------------*/

 v_col_var v_vars;
 int NameF;       ///< name (number) of the function at point Lambda
 Index NrCmp;     ///< number of component functions
 int seed;        ///< seed for random number generation

 dblVR1 bQR;
 dblVR1 aQR;
 dblVR2 cQR;

 Index GPMaxSz;

 std::vector< FunctionValue > SubG;
 double FiVal;

 std::mt19937 rg;  ///< base random generator

/*--------------------------------------------------------------------------*/

 const static std::vector< int > dflt_int_par;
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

 LukFiBlock( Block *father = nullptr ) : Block( father ) , AR( 0 ) {}

/*--------------------------------------------------------------------------*/
 /// destructor of LukFiBlock
 /** Destructor of LukFiBlock. . */

 virtual ~LukFiBlock() { f.clear(); }

/** @} ---------------------------------------------------------------------*/
/*-------------------------- OTHER INITIALIZATIONS -------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Other initializations
 *  @{ */

 /// load the LukFiBlock out of an istream
 /** Load the LukFiBlock out of an istream. The format is:
  *
  * number of variables ,
  * name of the function
  * number of components ,
  * seed
  *
  * Since only one format is supported, \p frmt is ignored. */

 void load( std::istream & input , char frmt = 0 ) override;

/*--------------------------------------------------------------------------*/
 /// extends Block::deserialize( netCDF::NcGroup )
 /** Extends Block::deserialize( netCDF::NcGroup ) to the specific format of
  * a LukFiBlock.  */

 void deserialize( const netCDF::NcGroup & group ) override;

/*--------------------------------------------------------------------------*/

 void generate_abstract_variables( Configuration *stvv = nullptr ) override;

/*--------------------------------------------------------------------------*/

 void generate_objective( Configuration *objc = nullptr ) override;

/** @} ---------------------------------------------------------------------*/
/*------------- METHODS FOR PRINTING & SAVING THE LukFiBlock ---------------*/
/*--------------------------------------------------------------------------*/
/** @name Methods for printing & saving the LukFiBlock
 *  @{ */

 /// print the LukFiBlock on an ostream with the given verbosity

 void print( std::ostream & output , char vlvl = 0 ) const override;

/*--------------------------------------------------------------------------*/

 void serialize( netCDF::NcGroup & file ) const override;

/** @} ---------------------------------------------------------------------*/
/*-------------------------------- ACCESSORS -------------------------------*/
/*--------------------------------------------------------------------------*/
/** @name Accessors
 *  @{ */

 std::vector< ColVariable > & get_x( void ) { return( x ); }

/** @} ---------------------------------------------------------------------*/
/*-------------------- PROTECTED PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

 protected:

/*--------------------------------------------------------------------------*/
/*-------------------------- PROTECTED METHODS -----------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------- PROTECTED FIELDS  ----------------------------*/
/*--------------------------------------------------------------------------*/

 std::vector< ColVariable > x;       /// the variables
 FRealObjective f;                   /// the objective function

 int NameF;       ///< name (number) of the function at point Lambda
 int NrCmp;       ///< number of component functions
 int seed;        ///< seed for random number generation

 unsigned char AR;   ///< bit-wise coded: what abstract is there

 static constexpr unsigned char HasVar = 1;
 ///< first bit of AR == 1 if the Variable have been constructed
 static constexpr unsigned char HasObj = 2;
 ///< second bit of AR == 1 if the Objective has been constructed

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

 SMSpp_insert_in_factory_h;        // insert it in the Block factory

/*--------------------------------------------------------------------------*/

};  // end( class( LukFiBlock ) )

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

 }  // end( namespace SMSpp_di_unipi_it )

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

#endif  /* LukFiBlock.h included */

/*--------------------------------------------------------------------------*/
/*--------------------- End File LukFiBlock.h ------------------------------*/
/*--------------------------------------------------------------------------*/
