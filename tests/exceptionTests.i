// -*- lsst-c++ -*-
%define exceptionTests_DOCSTRING
"
Tests for throwing exceptions from C++ to Python
"
%enddef

%feature("autodoc", "1");
%module(docstring=exceptionTests_DOCSTRING) exceptionTests

%{
#include "lsst/mwi/exceptions.h"
%}

%include "lsst/mwi/p_lsstSwig.i"

%define %lsst_throw(Type)
    %inline %{
    void throw##Type() {
        throw lsst::mwi::exceptions::Type("Error Message");
    }
    %}
%enddef

%lsst_throw(DomainError);
%lsst_throw(InvalidParameter);
%lsst_throw(LengthError);
%lsst_throw(OutOfRange);
%lsst_throw(Runtime);
%lsst_throw(RangeError);
%lsst_throw(Overflow);
%lsst_throw(Underflow);
%lsst_throw(NotFound);
%lsst_throw(Memory);
%lsst_throw(FitsError);

%inline %{
void throwFirst() {
    throw lsst::mwi::exceptions::Overflow("bang");
}

void throwSecond(const char * const name) {
    using namespace lsst::mwi::data;
    using namespace lsst::mwi::exceptions;
    try {
        throwFirst();
    } catch (ExceptionStack & e) {
        ExceptionData ed(name);
        ed << DataProperty::PtrType(new DataProperty("do not", boost::any(std::string("panic"))));
        e << ed;
        throw lsst::mwi::exceptions::OutOfRange(e, "boom");
    }
}

void throwChain(const char * const name) {
    using namespace lsst::mwi::exceptions;
    try {
        throwSecond(name);
    } catch (ExceptionStack & e) {
        throw InvalidParameter(e, "crash");
    }
}
%}
