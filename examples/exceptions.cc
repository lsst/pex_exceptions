// -*- lsst-c++ -*-
/** \file exception.cc
  * \brief Example of the use of base LSST exception classes
  * \author Roberta Allsman
  */

#include <iostream>
#include "lsst/pex/exceptions.h"
#include "lsst/pex/logging/Trace.h"

using namespace lsst::daf::base;
using namespace lsst::pex::logging;
using namespace lsst::pex::exceptions;

void doSecondFunction() { 

    Trace( "lsst", 1, "Entered doSecondFunction(), building exception to throw");

    //Create ExceptionData with informative attributes used during later debug 
    ExceptionData nfNode("nfNode");

    DataProperty::PtrType dp (new DataProperty(std::string("MyKey"),boost::any( std::string( "nfNode:MyKey Value" ))));
    nfNode << dp;
    nfNode << DataProperty::PtrType( new DataProperty( std::string("WHY" ),
                      boost::any( std::string( "nfNode:Why exception message" ))))
           << DataProperty::PtrType( new DataProperty( std::string("KEY1_1" ),
                      boost::any( std::string("nfNode:KEY1_attribute" ))))
           << DataProperty::PtrType( new DataProperty( std::string("KEY1_2" ),
                      boost::any((int) 13 )));

    Trace( "exceptions",1, boost::str(boost::format(
        "ExceptionData attributes before addition to stack:\n%s\n")
        % nfNode.getExceptionData()->toString("",true) ));
    
    try { 
      
      // Initialize Handler and its Exception Stack using ExceptionData prefabbed
      NotFound notFound(nfNode,"NotFound exception handling" );
      Trace( "exceptions",1, boost::str(boost::format(
            "NotFound stack after operator<<:\n%s\n")
            % (notFound.getStack())->toString("",true) ));
    

      ////////////////////////////////////////////////////////////////////
      // Miscellaneous testing
      // Test Cloning of exception collection managed by exception handler
      OutOfRange toBeCloned("Out Of Range Exception") ;
      Trace( "exceptions",1, boost::str(boost::format(
             "TEST: before: toBeCloned Handler object:\n%s\n")
             % toBeCloned.getStack()->toString("",true) ));
      toBeCloned.cloneCollection(notFound);
      Trace( "exceptions",1, boost::str(boost::format(
             "TEST: after toBeCloned Handler object:\n%s\n")
             % toBeCloned.getStack()->toString("",true) ));

      RangeError initAsCloned(toBeCloned,"Range Error Exception") ;
      Trace( "exceptions",1, boost::str(boost::format(
             "TEST: initAsCloned Handler object:\n%s\n")
             % initAsCloned.getStack()->toString("",true) ));
      ////////////////////////////////////////////////////////////////////


      Trace( "lsst", 1, "Forcing NotFound exception in doSecondFunction() ");
      // Force an Exception Error; provide updated ExceptionStack for catch
      throw notFound;

    } 
    //  Catch LsstException; clone of object passed on throw is provided.
    catch(NotFound &retNotFound) {
        Trace( "exceptions",1, boost::str(boost::format(
               "Caught NotFound in doSecondFunction()\n... error: %s\n%s\n") 
                % retNotFound.what() 
                % retNotFound.getStack()->toString("... ",true )));

        // Ensure that returned Exception Stack is a valid Node
        if (retNotFound.getStack()->isNode() == false) {
                Trace( "exception",1, "ERROR: Invalid ExceptionStack");
                exit(EXIT_FAILURE);
        }

        ////////////////////////////////////////////////////////////////////
        // Miscellaneous useful operations
        //  Sample1: Display all properties in each ExceptionData
        ExceptionStack::PtrType retEStack = retNotFound.getStack();
        ExceptionStack::iteratorRangeType range = retEStack->getChildren();
        ExceptionStack::ContainerType::const_iterator iterExceptions;
        int counter = 0;
        for ( iterExceptions = range.first; iterExceptions != range.second; iterExceptions++) {
                ++counter;
                if ((*iterExceptions)->isNode() == true) {
                    Trace( "exceptions",1,boost::format("Excp%d %s") 
                        % counter % (*iterExceptions)->toString("", false ) );
                    ExceptionStack::iteratorRangeType rangeProperty = (*iterExceptions)->getChildren();
                    ExceptionStack::ContainerType::const_iterator iterProperty;
                    int counterP = 0;
                    for (iterProperty = rangeProperty.first; 
                        iterProperty != rangeProperty.second; 
                        iterProperty++){
                           ++counterP;
                           Trace( "exceptions",1,boost::format("Prop%d  %s ") 
                                % counterP % (*iterProperty)->toString("") );
                    }
                }
        }
        ////////////////////////////////////////////////////////////////////


        // Sample2: Build new exception handler object using old stack, 
        //          build new ExceptionData, add to Stack, throw handler object
        Memory badMemory(retNotFound,"Testing:Memory Failure" );
        ExceptionData e2Node("memNode");
        e2Node << DataProperty::PtrType(new DataProperty(std::string("WHY"),
                            std::string("memNode:Why exception error message")))
               << DataProperty::PtrType( new DataProperty(std::string("KEY2_1"),
                                std::string("memNode:KEY2_attribute")));
        badMemory << e2Node;

        Trace( "exceptions",1, boost::str(boost::format(
            "Created Memory ExceptionStack to throw.\n%s\n")
            % badMemory.getStack()->toString("",true )));
        Trace( "lsst", 1, "Exiting doSecondFunction() on Memory throw");
        throw badMemory;
    }
}

void doFirstFunction() {
    try{
        Trace("exceptions",1,"Entered doFirstFunction()");

        doSecondFunction();

        Trace("exceptions",1,"Exited doSecondFunction()");
    } 
    // catch exception exit expected from doSecondFunction
    catch ( Memory &retMemory) {
        Trace("exceptions",1,"Caught Memory exception in doFirstFunction()");
        Trace( "exceptions",1, boost::str(boost::format(
               "Last in stack (label memNode)\n... error: %s\n%s\n") 
                % retMemory.what() % retMemory.getStack()->toString("... ",true )));
        // Sample3: extract WHY keyword from full ExceptionStack loaded
        ExceptionStack::PtrType retEStack = retMemory.getStack();
        DataProperty::nameSetType names = retEStack->findNames("WHY");
        for( DataProperty::nameSetType::iterator iterN = names.begin();
                iterN != names.end(); iterN++){
                Trace("exceptions",1,
                    boost::str(boost::format("... found name '%s'" ) % *iterN ));
                DataProperty::PtrType obj = retEStack->findUnique(*iterN);
                Trace("exceptions",1,
                    boost::str(boost::format("... object %s" ) % obj->toString(std::string(15,' ')) ));
        }

        // Sample4: extract WHY from LAST ExceptionData loaded in Stack
        ExceptionData::PtrType lastException = retMemory.getLast();

        // ... extract WHY comment(s) from this LAST exception
            DataProperty::nameSetType namesLastException = lastException->findNames("WHY");
        // ... Need to check that WHY keyword was found
        if ( namesLastException.empty() ) {
                Trace("exceptions",1,
                        "No WHY comment found in Last ExceptionData" )  ;
        } else { // loop thru full dotname of WHY comments, print results
                for( DataProperty::nameSetType::iterator iterW = 
                        namesLastException.begin(); 
                        iterW != namesLastException.end(); 
                        iterW++){
                    DataProperty::PtrType obj = lastException->findUnique( *iterW );
                    Trace("exceptions",1,
                        boost::str(boost::format("Extracted from LastException: %s:  %s" ) % *iterW % obj->toString(std::string(15,' ')) ));
                }
        }

    }
}

int main(int ac, char **av) {

  { // bracket census block to enable accurate check of notFound leaks at end of run.


    // On exiting the catch block, the ExceptionStack object is deleted. 
    // By convention, original error condition was satisfactorily handled if
    //    the catch block was exited (excepting another throw).

    int verbosity = 1;
    Trace::setVerbosity("", verbosity);
    Trace( "exception", 1, "Testing Lsst Exception handling");
    
    try {
        doFirstFunction();
    } catch (...) {
        Trace( "exception", 1, "Caught unexpected Exception");
        return EXIT_FAILURE;
    }

    // Test common forms of LsstException calling - no prior Exception setup
    try {
        Trace( "exception", 1, "\nTest throw of named exception with no prior setup");
        throw InvalidParameter( "This tested throw without prior setup" );
    }
    catch (lsst::pex::exceptions::InvalidParameter &invalidParam) {
        Trace( "exceptions",1, boost::str(boost::format(
               "Caught InvalidParameter \n... error: %s\nFullStack:\n%s\n") 
                % invalidParam.what() % invalidParam.getStack()->toString("... ",true )));
        Trace( "exceptions",1, boost::str(boost::format(
               "LastException:%s\n") 
                % invalidParam.getLast()->toString("... ",true )));
    }

    // Test common forms of LsstException calling - no prior ExceptionData setup
    try {
        Trace( "exception", 1, "\nTest throw of named exception whilst adding attributes on the fly");
        int randomValue = 5;
        throw DomainError( "Domain error with important attribute passed" )
                << DataProperty("KeyLabel1",boost::format("Value"))
                << DataProperty("KeyLabel2",randomValue);
    }
    catch (DomainError &domainError) {
        Trace( "exceptions",1, boost::str(boost::format(
               "Caught DomainError \n... error: %s\nFullStack:\n%s\n") 
                % domainError.what() % domainError.getStack()->toString("... ",true )));
        Trace( "exceptions",1, boost::str(boost::format(
               "LastException:%s\n") 
                % domainError.getLast()->toString("... ",true )));
    }

    // Test common forms of LsstException catching using ExceptionStack
    try {
        Trace( "exception", 1, "\nTest catch of generic LsstException no knowing exception type");
        throw InvalidParameter( "hidden message: InvalidParameter thrown" );
    }
    catch (ExceptionStack &e) {
        Trace( "exceptions",1, boost::str(boost::format(
               "Caught an LsstException but don't know type \n... error: %s\nFullStack:\n%s\n") 
                % e.what() % e.getStack()->toString("... ",true )));
        Trace( "exceptions",1, boost::str(boost::format(
               "LastException:%s\n") 
                % e.getLast()->toString("... ",true )));
    }

  }     
  //
  // End census block, now check for memory leaks
  //
  if (Citizen::census(0) == 0) {
        Trace( "exception",1, "No leaks detected");
  } else {
        Trace( "exception",1, "ERROR: Memory leaks detected!");
        Citizen::census(std::cerr);
  }

  return EXIT_SUCCESS;
}
