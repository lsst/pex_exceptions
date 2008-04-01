// -*- lsst-c++ -*-
#if !defined(LSST_RUNTIME_EXCEPTION)      //! multiple inclusion guard macro
#define LSST_RUNTIME_EXCEPTION 1
dnl "dnl" is the m4 comment syntax
dnl
undefine(`format')dnl ' Stop m4 expanding format
//
dnl comment to go into the output file
// This file is machine generated from Runtime.h.m4. Please do not edit directly
//         
dnl
/** \file  Runtime.h
  * \brief LSST exception handler classes for generic runtime exception events
  *
  * When LSST Data Managment software forces (i.e. throws) an exception 
  * event, an LSST exception handler (e.g. OutOfRange) must be invoked.  
  * When DM software catches an exception event, the software either 
  * satisfactorily resolves the event and continues; or ensures that 
  * regardless of the type of event caught, the object re-thrown is an LSST 
  * exception handler.
  *
  * An LSST exception handler manages an LSST ExceptionStack composed of 
  * an ordered collection of LSST ExceptionDatas. Each ExceptionData within the 
  * ExceptionStack represents the occurence of an exception event which 
  * couldnt be resolved and was rethrown in anticipation the next level 
  * would succeed. When a handler successfully resolves the event and 
  * normal processing continues, the exception stack goes out of scope and 
  * is deleted.
  
  * Each ExceptionData is composed of attributes providing additional debugging 
  * information to the exception handler catching that specific 
  * error event. Those attributes are specified using the DataProperty class.
  * An ExceptionData is a collection of DataProperty objects.
  
  * When the current exception handler prepares to throw an exception, 
  * it creates an LSST ExceptionData object, adds it to the LSST ExceptionStack 
  * originally provided (or creates a new LSST ExceptionStack), 
  * then initializes the appropriate LSST exception handler with the 
  * ExceptionStack. Finally the current handler throws the newly
  * initialized exception handler.
  *
  * See: DataProperty, ExceptionData, ExceptionStack.
  *
  * The base exception handlers include:
  * - Logic Errors
  *   -  DomainError : Parameter or argument outside expected mathematical domain
  *   -  InvalidParameter : Invalid parameter or argument
  *   -  LengthError : New object exceeds maximum permitted size
  *   -  OutOfRange : Parameter or argument not within expected range
  * - Runtime Errors
  *   -  Runtime : Generic runtime error
  *   -  RangeError : Internal computation resulted in range error
  *   -  Overflow  : Internal computation resulted in overflow
  *   -  Underflow : Internal computation resulted in underflow
  * - General Purpose Errors
  *   -  NotFound : Resource not found
  *   -  Memory :  Memory system failure
  *   -  FitsError : Problem in FITS I/O system
  *
  * \ingroup pex
  *
  * \ author Robert Lupton
  * \ author Roberta Allsman
  */
#include <exception>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include "lsst/daf/base/DataProperty.h"
#include "lsst/pex/exceptions/Exception.h"


namespace lsst {
namespace pex {
namespace exceptions {

        dnl
        dnl define(name, body) defines an m4 macro
        dnl Define a new subclass $1 of ExceptionData without added functionality; docstring $2
        define(LSST_NEW_EXCEPTION,
`
/** \brief $1 handler thrown or caught on an exceptional event.
  *
  * The handlers "catch" indicates the following situation occurred: 
  * $2
  */
class $1 : public ExceptionStack  {
public:
  /** Construct new $1 object.
    *
    * \param comment  String used to initialize std::runtime_error.what(). 
    * \note         Default ExceptionStack and ExceptionData will be created
    */
    $1(std::string const& comment) throw() :
         ExceptionStack(std::string("$1"),comment) {
         ExceptionData eNode("Node$1");
         this->addExceptionData(eNode);
    };


  /** Construct new $1 object.
    *
    * \param comment  String used to initialize std::runtime_error.what(). 
    * \note         Default ExceptionStack and ExceptionData will be created
    */
    $1(boost::format const& comment) throw() :
         ExceptionStack( std::string("$1"),comment.str()) {
         ExceptionData eNode("Node$1");
         this->addExceptionData(eNode);
    };

  /** Construct new $1 object.
    *
    * \param orig     Reference to ExceptionData to be cloned for use in ExceptionStack.
    * \param comment  String used to initialize std::runtime_error.what(). 
    *                 Default to: "$2".
    */
    $1(ExceptionData & orig, std::string const& comment=std::string("$2")) 
        throw() :
        ExceptionStack( std::string("$1"), comment) {
        this->addExceptionData(orig);
    };

  /** Construct new $1 object.
    *
    * \param orig     Reference to ExceptionData to be cloned for use in ExceptionStack.
    * \param comment  String used to initialize std::runtime_error.what(). 
    *                 Default to: "$2".
    */
    $1(ExceptionData & orig, boost::format const& comment=boost::format("$2")) 
        throw() :
        ExceptionStack(orig, std::string("$1"), comment.str()) {
        this->addExceptionData(orig);
    };

  /** Construct new $1 object.
    *
    * \param orig     Reference to ExceptionStack to be cloned for use.
    * \param comment  String used to initialize std::runtime_error.what(). 
    *                 Default to: "$2".
    */
    $1(ExceptionStack const& orig, std::string const& comment=std::string("$2")) throw() :
         ExceptionStack( orig, std::string("$1"), comment) {
    };

  /** Construct new $1 object.
    *
    * \param orig     Reference to ExceptionStack to be cloned for use.
    * \param comment  String used to initialize std::runtime_error.what(). 
    *                 Default to: "$2".
    */
    $1(ExceptionStack const& orig, boost::format const& comment=boost::format("$2")) throw() :
         ExceptionStack(orig, std::string("$1"), comment.str()){
    };


  /** $1 copy constructor.  This is a clone operation.
    *
    * \param orig  A reference to the $1 object to clone.
    */
    $1(const $1 & orig) throw() :
         ExceptionStack(orig )  {};

  /** operator<< 
    *
    * \param rhs     Reference to ExceptionData to be added to $1 ExceptionStack .
    */
    $1 &operator<< (ExceptionData  rhs) throw() {
        this->getStack()->addProperty(rhs.getExceptionData());
        return *this;
    }

  /** operator<< 
    *
    * \param rhs     Reference to DataProperty to be added to most recent ExceptionData on ExceptionStack.
    */
    $1 &operator<< (lsst::daf::base::DataProperty  const rhs) throw() {
        this->getLast()->addProperty(rhs);
        return *this;
    }

  /** operator<< 
    *
    * \param rhs     Reference to DataProperty to be added to most recent ExceptionData on ExceptionStack.
    */
    $1 &operator<< (lsst::daf::base::DataProperty::PtrType  const rhs) throw() {
        this->getLast()->addProperty(rhs);
        return *this;
    }


  /** Destructor for $1 object
    */
    ~$1() throw() {}

    virtual char const * getPythonClass() const { return "Lsst$1"; }

}')

// Mapping std:logical_error
    LSST_NEW_EXCEPTION(DomainError,
                       Parameter or argument outside expected mathematical domain);

    LSST_NEW_EXCEPTION(InvalidParameter,
                       Invalid  parameter or argument);

    LSST_NEW_EXCEPTION(LengthError,
                       New object exceeds maximum permitted size);

    LSST_NEW_EXCEPTION(OutOfRange,
                       Parameter or argument not within expected range);


// Mapping std::runtime_error 
    LSST_NEW_EXCEPTION(Runtime,
                       Generic Runtime exception error);

    LSST_NEW_EXCEPTION(RangeError,
                       Internal computation resulted in range error);

    LSST_NEW_EXCEPTION(Overflow,
                       Internal computation resulted in overflow );

    LSST_NEW_EXCEPTION(Underflow,
                       Internal computation resulted in underflow );


// Mapping miscellaneous generic LSST errors
    LSST_NEW_EXCEPTION(NotFound,
                       Resource not found);

    LSST_NEW_EXCEPTION(Memory,
                       Memory system failure);

    LSST_NEW_EXCEPTION(FitsError,
                       Problem in FITS I/O system);

}
}
}
#endif
