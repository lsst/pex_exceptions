// -*- lsst-c++ -*-

/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */
 
%define pex_exceptions_DOCSTRING
"
Access to the classes from the pex_exceptions library
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.pex.exceptions", docstring=pex_exceptions_DOCSTRING) exceptionsLib

%{
#include <sstream>
#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"
%}

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

%ignore lsst::pex::exceptions::Exception::addToStream;

%newobject lsst::pex::exceptions::Exception::clone;
%immutable lsst::pex::exceptions::Tracepoint::_file;
%immutable lsst::pex::exceptions::Tracepoint::_func;
%template(Traceback) std::vector<lsst::pex::exceptions::Tracepoint>;

// Don't want this callable from Python, as Python does its own traceback-tracking.
%ignore lsst::pex::exceptions::addMessage;

%include "lsst/pex/exceptions/Exception.h"
%include "lsst/pex/exceptions/Runtime.h"

%extend lsst::pex::exceptions::Exception {
    std::string asString() {
        std::ostringstream stream;
        self->addToStream(stream);
        return stream.str();
    }
}

%types(lsst::pex::exceptions::LogicError *);
%types(lsst::pex::exceptions::DomainError *);
%types(lsst::pex::exceptions::InvalidParameterError *);
%types(lsst::pex::exceptions::LengthError *);
%types(lsst::pex::exceptions::OutOfRangeError *);
%types(lsst::pex::exceptions::RuntimeError *);
%types(lsst::pex::exceptions::RangeError *);
%types(lsst::pex::exceptions::OverflowError *);
%types(lsst::pex::exceptions::UnderflowError *);
%types(lsst::pex::exceptions::NotFoundError *);
%types(lsst::pex::exceptions::MemoryError *);
%types(lsst::pex::exceptions::IoError *);
%types(lsst::pex::exceptions::TypeError *);
%types(lsst::pex::exceptions::TimeoutError *);
