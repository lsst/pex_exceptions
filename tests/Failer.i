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
