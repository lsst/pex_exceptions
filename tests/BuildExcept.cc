// -*- lsst-c++ -*-
/** \file BuildExcept.cc
  * \brief Test base LSST exception classes
  * \author Roberta Allsman
  */

#include <string>
#include <iostream>
#include "lsst/pex/exceptions.h"
#include "lsst/pex/logging/Trace.h"
#include "lsst/daf/base.h"

using namespace lsst::daf::base;
using namespace lsst::pex::logging;
using namespace lsst::pex::exceptions;
using boost::any_cast;
using boost::shared_ptr;
using boost::regex;

int main(int argc, char *argv[])
{ // Census bracket

    int verbosity = 1;
    Trace::setVerbosity("", verbosity);
    Trace( "exception", 1, "Test Lsst Exception handling");

try { 
    Trace( "exception", 1, "\nTest minimalist exception handler setup." );
    throw OutOfRange("My OutOfRange Message");

} catch  (OutOfRange(&e))  {
    Trace( "exception", 1, boost::format(
           "Caught OutOfRange exception \nError:%s\nStack:\n%s" )
           % e.what() % e.getStack()->toString("...",true));
}


try {
    Trace( "exception", 1, 
            "\nTest Exception handler initialized with single Exception");

    // Create empty collection of user-defined exception attributes  
    ExceptionData eNode("NF");
    // Add exception comment to ExceptionData
    eNode << DataProperty::PtrType( new DataProperty(
                std::string("WHY" ),
                std::string("exception msg: Threw a  NotFound exception" ) ) );
    // Create attribute of some other relevant datum
    int datum = 64;
    DataProperty::PtrType eDatum( new DataProperty(
                std::string("MyDatum" ),
                datum) );
    eNode << eDatum;

    // Build Exception handler object adding exception details just created
    NotFound nf(eNode,"Message from NotFound init");
    // Cause the specific exception handler to be invoked

    Trace( "exception", 1, boost::format(
            "Will throw NotFound exception \nError will be:%s\nStack:\n%s" )
            % nf.what() % nf.getStack()->toString("...",true ) );
    throw nf;

} catch(NotFound &e){
    Trace( "exception", 1, boost::format(
            "Caught NotFound exception\nError:%s\nStack:\n%s" )
            % e.what() % e.getStack()->toString("...",true ) );

    try {
        Trace( "exception", 1, 
               "\nTest Exception handler initialized with ExceptionStack");
        
        // Build complex ExceptionData attribute structure
        ExceptionData rootNode("memNode");
        rootNode << DataProperty::PtrType( new DataProperty(
                        std::string("WHY" ),
                        std::string("memNode: Threw Memory exception" ) ) )
                 << DataProperty::PtrType(new DataProperty(
                        std::string("NPlane"),(int)5));
        DataProperty::PtrType bNode = 
                        DataProperty::createPropertyNode("bNode");
        DataProperty::PtrType cProperty(new DataProperty(
                        "sonofMaxPlane",(int)12));
        bNode->addProperty(cProperty);
        rootNode << bNode;
        Memory memory(e,"Memory Failure Message");
        memory << rootNode;

        Trace( "exception", 1, boost::format(
                "Will throw Memory exception \nError will be:%s\nStack:\n%s" )
                % memory.what() % memory.getStack()->toString("...",true));
        
        throw memory;
    } catch (Memory &retMemory){
        Trace( "exception", 1, boost::format(
               "Caught Memory exception \nError:%s\nStack:\n%s\n" )
               % retMemory.what() % retMemory.getStack()->toString("...",true));
    
        Trace( "exception", 1,  boost::format(
               "Extracting Last Exception from Stack:\n%s" )
               % retMemory.getLast()->toString("...",true));
    
        Trace( "exception", 1,
            "\nFinding keyword(s) matching regex (Pla.*) from Last Exception");
        DataProperty::PtrType lastException = retMemory.getLast();
        DataProperty::nameSetType names = lastException->findNames( "Pla.*" );
        if (  names.empty() == false ) {
            for (DataProperty::nameSetType::iterator pos = names.begin(); 
                pos != names.end();
                pos++ ) {
                DataProperty::PtrType obj = lastException->findUnique( *pos );
                 Trace( "exception", 1, boost::format("Property found: %s" )
                        % obj->toString(std::string(""), true) );
            }
        } else {
             Trace( "exception", 1,"Did NOT find matching keyword (Pla.*)");
        }
    } // end inner catch
} // end outer catch


//
// Check for memory leaks
//
if (Citizen::census(0) == 0) {
    Trace( "exception", 1,"\nNo leaks detected");
} else {
    Trace( "exception", 1,"\nLeaked memory blocks:");
    Citizen::census(std::cerr);
} 
return EXIT_SUCCESS;


}
