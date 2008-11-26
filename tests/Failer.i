// -*- lsst-c++ -*-
%define Failer_DOCSTRING
"
Access to the classes from the lsst_failer library
"
%enddef

%feature("autodoc", "1");
%module(package="failer", docstring=Failer_DOCSTRING) failer

%{
#include "Failer.h"
%}

%include "lsst/p_lsstSwig.i"

%lsst_exceptions()

%import "lsst/pex/exceptions/exceptionsLib.i"
%include "Failer.h"
