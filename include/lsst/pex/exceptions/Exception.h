// -*- lsst-c++ -*-
/** \file Exception.h
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
  * \ingroup pex
  *
  * \author Roberta Allsman 
  */

#if !defined(LSST_EXCEPTION)      //! multiple inclusion guard macro
#define LSST_EXCEPTION 1
#include <exception>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include "lsst/daf/base/DataProperty.h"

namespace lsst {
namespace pex {
namespace exceptions {

        
/** \brief ExceptionData is an ordered collection of (name,value) attributes.

  * Each ExceptionData is composed of attributes providing additional 
  * debugging information for the exception handler catching that 
  * specific exception. Those attributes are defined using the LSST 
  * DataProperty class.  An ExceptionData is a sequenced collection of pointers
  * to Data Property objects.
  *
  * ExceptionData inherits from and is a thin wrapper for 
  * lsst::daf::base::DataProperty. The rename assists in tracking the 
  * conceptually distinct DataProperty collections used in LSST 
  * exception management:
  * -     ExceptionStack = sequenced set { ->ExceptionData }
  * -     ExceptionData = sequenced set { ->DataProperty } .
  * -     DataProperty = leaf(name,value) || node(name, ->DataProperty)
  *
  * See DataProperty class for additional information.
  */
class ExceptionData   {
public:
   typedef lsst::daf::base::DataProperty::PtrType PtrType;
   typedef std::list<PtrType> ContainerType;
   typedef std::list<PtrType>::const_iterator ContainerIteratorType;
   typedef std::pair<ContainerIteratorType, ContainerIteratorType> iteratorRangeType;

   ExceptionData(const std::string name ) throw();
   ExceptionData(const boost::format name ) throw();
   ExceptionData(ExceptionData const& orig ) throw();
   PtrType getExceptionData() throw();
   void add(lsst::daf::base::DataProperty::PtrType rhs ) throw() ;
   ExceptionData &operator<< ( PtrType  rhs ) throw();
private:
    PtrType _exception;
};


/** \brief ExceptionStack manages an ordered collection of ExceptionData.
  *
  * An ExceptionStack manages a collection of ExceptionData objects using the
  * DataProperty (tree) paradigm so that the root Node is the exception stack,
  * second level nodes are ExceptionDatas and all subsequent nodes define
  * attributes on an exception.
  *
  * Providing uniquely named wrappers assists in distinguishing the various
  * DataProperty collections used in Lsst exception management:
  * -       ExceptionStack = sequenced set { ->ExceptionData }
  * -       ExceptionData = sequenced set { ->DataProperty }
  * -       DataProperty = leaf(name,value) || node(name, ->DataProperty)
  *
  * See ExceptionData and DataProperty classes for additional information.
  */
class ExceptionStack : public std::runtime_error  {
public:
   typedef lsst::daf::base::DataProperty::PtrType PtrType;
   typedef std::list<PtrType> ContainerType;
   typedef std::list<PtrType>::const_iterator ContainerIteratorType;
   typedef std::pair<ContainerIteratorType, ContainerIteratorType> iteratorRangeType;

   ExceptionStack( ) throw();
   ExceptionStack( std::string const& stackName, 
                   std::string const& comment=std::string("") )
                   throw();
   ExceptionStack( ExceptionStack const& orig,
                   std::string const& stackName, 
                   std::string const& comment=std::string("") ) 
                   throw();
   ExceptionStack( ExceptionData & orig,
                   std::string const& stackName, 
                   std::string const& comment=std::string("") ) 
                   throw();
   ExceptionStack( ExceptionStack const& orig ) throw();
   ExceptionStack &operator<<( ExceptionData rhs ) throw() ;
   ~ExceptionStack() throw() {};

   void            cloneCollection(ExceptionStack const& orig ) throw();
   void            addExceptionData(ExceptionData & orig) throw();
   PtrType         getStack() throw();
   PtrType         getLast() throw();

   virtual char const * getPythonModule() const;
   virtual char const * getPythonClass() const;

private:
   PtrType _exceptionStack;
};

}
}
}
#endif
