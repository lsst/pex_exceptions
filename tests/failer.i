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

%pythoncode %{
    import lsst.pex.exceptions
%}

%{
#include <new>
#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"
%}

// Use the Python C API to raise an exception of type
// lsst.pex.exceptions.Exception with a value that is a SWIGged proxy for a
// copy of the exception object.
%{
static void raiseLsstException(lsst::pex::exceptions::Exception& ex) {
    PyObject* pyex = 0;
    swig_type_info* tinfo = SWIG_TypeQuery(ex.getType());
    if (tinfo != 0) {
        lsst::pex::exceptions::Exception* e = ex.clone();
        pyex = SWIG_NewPointerObj(static_cast<void*>(e), tinfo,
            SWIG_POINTER_OWN);
    } else {
        pyex = Py_None;
    }

    PyObject* pyexbase = PyExc_RuntimeError;
    PyObject* module = PyImport_AddModule("lsst.pex.exceptions");
    if (module != 0) {
        pyexbase = PyObject_GetAttrString(module, "LsstCppException");
        if (pyexbase == 0) {
            pyexbase = PyExc_RuntimeError;
        }
    }

    PyErr_SetObject(pyexbase, pyex);
}

%}

// Turns on the default C++ to python exception handling interface
%define %lsst_exceptions()
    %exception {
        try {
            $action
        } catch (lsst::pex::exceptions::Exception &e) {
            raiseLsstException(e);
            SWIG_fail;
        } catch (std::exception & e) {
            PyErr_SetString(PyExc_Exception, e.what());
            SWIG_fail;
        }
    }
%enddef

%lsst_exceptions()

%import "lsst/pex/exceptions/exceptionsLib.i"
%include "Failer.h"
