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

%include "lsst/p_lsstSwig.i"

%include "lsst/pex/exceptions/Exception.h"
%include "lsst/pex/exceptions/Runtime.h"
