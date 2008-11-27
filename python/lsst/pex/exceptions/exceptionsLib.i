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
%}

%pythoncode {
class LsstException(Exception):
    pass

class LsstCppException(LsstException):
    def __str__(self):
        return self.args[0].what()

}

%include "lsst/p_lsstSwig.i"

%newobject lsst::pex::exceptions::Exception::clone;
%template(Traceback) std::vector<lsst::pex::exceptions::Tracepoint>;

%include "lsst/pex/exceptions/Exception.h"
%include "lsst/pex/exceptions/Runtime.h"
