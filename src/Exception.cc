// -*- lsst-c++ -*-
/** \file  Exception.cc
  * \brief Base LSST exception classes
  *
  * The LSST base exception classes provide the means to maintain an
  * exception stack trace as an unresolved error condition is successively
  * handed up the exception handler stack.
  *
  * Each handler is responsible for adding new information to the exception
  * stack which the next invoked handler may use to further resolve the issue.
  * A handler has access to the entire ExceptionStack so it is not
  * necessary to repeat information determined to already be in the stack.
  *
  * The exception stack is an ordered collection of ExceptionDatas which itself
  * is an ordered collection of (name, value) attributes pairs.
  *
  * The conceptually distinct collections used in LSST exception management
  * are:
  * -     ExceptionStack = sequenced set { ->ExceptionData }
  * -     ExceptionData = sequenced set { ->DataProperty } .
  * -     DataProperty = leaf(name,value) || node(name, ->DataProperty)
  *
  * ExceptionStack and ExceptionData are implemented as wrappers for
  * DataProperty collections.
  *
  * See ExceptionStack, ExceptionData and DataProperty classes for
  * additional information.
  *
  *
  * \defgroup pex_exceptions Pipeline Execution Exceptions
  *
  * \ingroup pex_exceptions
  *
  * \author Roberta Allsman
  */

#include <exception>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include "lsst/daf/base/DataProperty.h"
#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"

using namespace lsst::daf::base;
using namespace lsst::pex::logging;

const std::string nullExceptionMsg("Null exception message");

//////////////////////////////////////////////////////////////////////////////
//  E x c e p t i o n                 CONSTRUCTORS, COPY OPERATORS, DESTRUCTORS
//////////////////////////////////////////////////////////////////////////////

/** Construct a new ExceptionData object.
  *
  * \param  name  The default root node name of the ExceptionData object.
  * See DataProperty for more information.
  */
lsst::pex::exceptions::ExceptionData::ExceptionData( const std::string name ) 
    throw() :
    _exception( DataProperty::createPropertyNode(name) ) {
};

/** Construct a new ExceptionData object.
  *
  * \param name   The default root node name of the ExceptionData object.
  * See DataProperty for more information.
  */
lsst::pex::exceptions::ExceptionData::ExceptionData( const boost::format name ) 
    throw() :
    _exception(DataProperty::createPropertyNode(name.str() )) {
};

/**  ExceptionData copy constructor
  *
  * This is a clone operation. The resulting ExceptionData object will be
  * completely independent of the source object, including all descendants.
  *
  * \param orig A reference to the ExceptionStack to clone.
  */
lsst::pex::exceptions::ExceptionData::ExceptionData( ExceptionData const& orig ) 
    throw() :
    _exception(DataProperty::PtrType( new DataProperty(*orig._exception) ) ) {
};


//////////////////////////////////////////////////////////////////////////////
//  E x c e p t i o n                 Methods
//////////////////////////////////////////////////////////////////////////////

/**  Return exception reference
  *
  * \return A shared pointer to the ExceptionData.
  */
DataProperty::PtrType lsst::pex::exceptions::ExceptionData::getExceptionData() throw() {
    return _exception;
}


/**   Add more attributes to the ExceptionData
  *
  * \param rhs A shared pointer to the DataProperty attribute.
  */
void lsst::pex::exceptions::ExceptionData::add( lsst::daf::data::DataProperty::PtrType  rhs) 
    throw() { 
    _exception->addProperty(rhs);
}

/**   Operator<< :  Add more attributes to the ExceptionData
  *
  * \param rhs A shared pointer to the DataProperty attribute.
  */
lsst::pex::exceptions::ExceptionData &lsst::pex::exceptions::ExceptionData::operator<< (PtrType  const rhs) 
    throw() { 
    _exception->addProperty(rhs);
    return *this;
}



//////////////////////////////////////////////////////////////////////////////
// E x c e p t i o n S t a c k        CONSTRUCTORS, COPY OPERATORS, DESTRUCTORS
//////////////////////////////////////////////////////////////////////////////


/** Construct a new ExceptionStack object.
  *
  * \note   Default runtime_error comment is: "Null exception message".
  * \note   Default root node name of the ExceptionStack object is: "eStack".
  */
lsst::pex::exceptions::ExceptionStack::ExceptionStack() 
    throw() : 
    std::runtime_error(nullExceptionMsg) {
    _exceptionStack = SupportFactory::createPropertyNode("eStack" );
}

/** Construct a new ExceptionStack object.
  *
  * \param stackName The name of the object. 
  *                  Syntax conforms to lsst::daf::DataProperty(name) 
  *                  specification; any '.' characters in the stackName are 
  *                  disallowed and will be replaced with '@' characters.
  * \param comment   The string used to initialize std::runtime_error.what() 
  *                  return. Default comment is: "Null exception message".
  */
lsst::pex::exceptions::ExceptionStack::ExceptionStack(std::string const& stackName, 
                                                      std::string const& comment) 
    throw() : 
    std::runtime_error(comment) {
    _exceptionStack = DataProperty::createPropertyNode(stackName );
}
/** Construct a new ExceptionStack object.
  *
  * \param orig      A reference to the ExceptionStack to clone.
  * \param stackName Label for the ExceptionStack root node. 
  *                  Syntax conforms to lsst::daf::DataProperty(name) 
  *                  specification; any '.' characters in the stackName are 
  *                  disallowed and will be replaced with '@' characters.
  * \param comment   The string used to initialize std::runtime_error.what() 
  *                  return. Default comment is: "Null exception message".
  *
  * TBD:  When nodes can be renamed in place, rename the cloned node to
  *       stackName .  DataProperty being updated to allow this. 21 Aug 07
  */
lsst::pex::exceptions::ExceptionStack::ExceptionStack( ExceptionStack const& orig,
                                                       std::string const& stackName, 
                                                       std::string const& comment ) 
    throw() : 
    std::runtime_error(comment ) {
    _exceptionStack = DataProperty::PtrType( new DataProperty(*orig._exceptionStack) );
}


/** Construct a new ExceptionStack object.
  *
  * \param orig      A reference to the ExceptionData to clone.
  * \param stackName Label for the ExceptionStack root node. 
  *                  Syntax conforms to lsst::daf::DataProperty(name) 
  *                  specification; any '.' characters in the stackName are 
  *                  disallowed and will be replaced with '@' characters.
  * \param comment   The string used to initialize std::runtime_error.what() 
  *                  return. Default comment is: "Null exception message".
  *
  * TBD:  When nodes can be renamed in place, rename the cloned node to
  *       stackName .  DataProperty being updated to allow this. 21 Aug 07
  */
lsst::pex::exceptions::ExceptionStack::ExceptionStack( ExceptionData & orig,
                                                       std::string const& stackName, 
                                                       std::string const& comment ) 
    throw() : 
    std::runtime_error(comment ) {
    _exceptionStack = DataProperty::createPropertyNode(stackName );
    DataProperty::PtrType exception = orig.getExceptionData();
    _exceptionStack->addProperty( *exception );
}

/** ExceptionStack copy constructor.
  *
  * This is a clone operation. The resulting ExceptionStack object will be
  * completely independent of the source object, including all descendants.
  *
  * \param orig      A reference to the ExceptionStack to clone.
  *
  */
lsst::pex::exceptions::ExceptionStack::ExceptionStack( ExceptionStack const& orig  ) 
    throw() :
    std::runtime_error(orig.what() ) {
    _exceptionStack = DataProperty::PtrType( new DataProperty(*orig._exceptionStack) );
}

/** Load exception collection into ExceptionStack
  *
  * This is a clone operation. The resulting _exceptionStack collection will 
  * be completely independent of source object, including all descendants.
  *
  * This operation does NOT update the exception error comment.
  *
  * \param orig      A reference to the DataProperty::ExceptionData::PtrType to 
  *                  clone.
  *
  */
void lsst::pex::exceptions::ExceptionStack::cloneCollection(ExceptionStack const& orig  ) 
    throw()  {
    _exceptionStack = DataProperty::PtrType( new DataProperty(*(orig._exceptionStack )));
}

//////////////////////////////////////////////////////////////////////////////
// E x c e p t i o n S t a c k        Auxilary Methods
//////////////////////////////////////////////////////////////////////////////

/** Add an ExceptionData to ExceptionStack.
  *
  */
void lsst::pex::exceptions::ExceptionStack::addExceptionData( ExceptionData & orig) 
    throw() {
    _exceptionStack->addProperty(*(orig.getExceptionData()));
}

/** Return a pointer to ExceptionData collection managed by ExceptionStack.
  *
  * \return A PtrTYpe containing a reference to a collection of ExceptionData.
  */
DataProperty::PtrType lsst::pex::exceptions::ExceptionStack::getStack() 
    throw() {
    return _exceptionStack;
}

/** Return a pointer to sequentially last ExceptionData added to ExceptionStack
  *
  * \return   a PtrType containing a reference to an ExceptionData object which 
  *           itself is a collection of DataProperties
  *
  * \throw Throws lsst::pex::exception::Runtime if this ExceptionStack does 
  *         not contain a least one ExceptionData object.
  */
DataProperty::PtrType lsst::pex::exceptions::ExceptionStack::getLast() 
    throw() {
    ExceptionStack::iteratorRangeType rangeExceptions = 
                                _exceptionStack->getChildren();
    if ( std::distance(rangeExceptions.first,rangeExceptions.second) == 0) {
        throw Runtime("Exception Stack contains no ExceptionData");
        }
    ExceptionStack::ContainerType::const_iterator lastExceptionPtr = 
                                rangeExceptions.second;
    return *(--lastExceptionPtr);     // 2cd iterator is one PAST list end
}

/**   Operator<< :  Add ExceptionData to the ExceptionStack
  *
  * \param rhs The ExceptionData to add to the ExceptionStack 
  */
lsst::pex::exceptions::ExceptionStack &lsst::pex::exceptions::ExceptionStack::operator<< ( ExceptionData rhs) 
    throw() { 
    _exceptionStack->addProperty(rhs.getExceptionData());
    return *this;
}

/** Returns the name of the Python module containing the Python class that the
  * ExceptionStack should be translated to when throwing from C++ to Python.
  */
char const * lsst::pex::exceptions::ExceptionStack::getPythonModule() const {
    return "lsst.pex.exceptions";
}

/** Returns the name of the Python class that the ExceptionStack
  * should be translated to when throwing from C++ to Python.
  */
char const * lsst::pex::exceptions::ExceptionStack::getPythonClass() const {
    return "LsstExceptionStack";
}
