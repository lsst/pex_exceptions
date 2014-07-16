// -*- lsst-c++ -*-
/*
 * LSST Data Management System
 * Copyright 2008-2014 LSST Corporation.
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

%define testLib_DOCSTRING
"
Test module for pex_exceptions
"
%enddef

%feature("autodoc", "1");
%module(package="testLib", docstring=testLib_DOCSTRING) testLib

%pythonnondynamic;
%naturalvar;  // use const reference typemaps

%include "cpointer.i"
%include "exception.i"
%include "std_list.i"
%include "std_map.i"
%include "std_string.i"
%include "std_set.i"
%include "std_vector.i"
%include "std_iostream.i"
%include "boost_shared_ptr.i"
%include "carrays.i"
%include "typemaps.i"

%{
#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"
%}

%include "lsst/pex/exceptions/handler.i"
%lsst_exceptions()

%import "lsst/pex/exceptions/exceptionsLib.i"

%inline {

template <typename T>
void fail1(std::string const & message) {
    throw LSST_EXCEPT(T, message);
}

template <typename T>
void fail2(std::string const & message1, std::string const & message2) {
    try {
        fail1<T>(message1);
    } catch (T & err) {
        LSST_EXCEPT_ADD(err, message2);
        throw err;
    }
}

}

%define %INSTANTIATE(NAME)
%template(fail ## NAME ## 1) fail1< lsst::pex::exceptions::##NAME >;
%template(fail ## NAME ## 2) fail2< lsst::pex::exceptions::##NAME >;
%enddef

%INSTANTIATE(Exception)
%INSTANTIATE(LogicError)
