// -*- lsst-c++ -*-
%define pex_exceptions_DOCSTRING
"
Access to the classes from the pex_exceptions library
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.pex.exceptions", docstring=pex_exceptions_DOCSTRING) exceptionsLib

%{
#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"
%}

%pythoncode {
class LsstException(Exception):
    pass

class LsstCppException(LsstException):
    def __str__(self):
        return self.args[0].what()

}

%pythonnondynamic;
%naturalvar;  // use const reference typemaps

%include "cpointer.i"
%include "exception.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_iostream.i"
%include "boost_shared_ptr.i"
%include "carrays.i"
%include "typemaps.i"

%newobject lsst::pex::exceptions::Exception::clone;
%template(Traceback) std::vector<lsst::pex::exceptions::Tracepoint>;

%include "lsst/pex/exceptions/Exception.h"
%include "lsst/pex/exceptions/Runtime.h"

%types(lsst::pex::exceptions::LogicErrorException *);
%types(lsst::pex::exceptions::DomainErrorException *);
%types(lsst::pex::exceptions::InvalidParameterException *);
%types(lsst::pex::exceptions::LengthErrorException *);
%types(lsst::pex::exceptions::OutOfRangeException *);
%types(lsst::pex::exceptions::RuntimeErrorException *);
%types(lsst::pex::exceptions::RangeErrorException *);
%types(lsst::pex::exceptions::OverflowErrorException *);
%types(lsst::pex::exceptions::UnderflowErrorException *);
%types(lsst::pex::exceptions::NotFoundException *);
%types(lsst::pex::exceptions::MemoryException *);
%types(lsst::pex::exceptions::IoErrorException *);
%types(lsst::pex::exceptions::TimeoutException *);
