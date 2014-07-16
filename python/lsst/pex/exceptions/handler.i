// -*- lsst-c++ -*-
/*
 * LSST Data Management System
 * Copyright 2008-20141 LSST Corporation.
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


// Use the Python C API to raise an exception of type
// lsst.pex.exceptions.Exception with a value that is a SWIGged proxy for a
// copy of the exception object.
%{

static void tryLsstExceptionWarn(const char * message) {
    // Try to warn that exception translation failed, if we fail, clear the exception raised by the
    // warning attempt so we can raise a less-informative exception based on the original.
    int s = PyErr_WarnEx(PyExc_Warning, message, 1);
    if (s) {
        PyErr_Clear();
    }
}

static void raiseLsstException(lsst::pex::exceptions::Exception& ex) {
    PyObject* pyex = 0;
    swig_type_info* tinfo = SWIG_TypeQuery(ex.getType());
    bool failed = false;
    if (tinfo != 0) {
	lsst::pex::exceptions::Exception* e = ex.clone();
        pyex = SWIG_NewPointerObj(static_cast<void*>(e), tinfo, SWIG_POINTER_OWN);
        if (!pyex) {
            tryLsstExceptionWarn("Could not Swig-convert C++ Exception.");
            failed = true;
        }
    } else {
        tryLsstExceptionWarn("C++ Exception not known to Swig.");
	failed = true;
    }

    PyObject* module = PyImport_ImportModule("lsst.pex.exceptions.wrappers");
    if (!module) {
        tryLsstExceptionWarn("Failed to import C++ Exception wrapper module.");
        failed = true;
    } else {
        PyObject* translate = PyObject_GetAttrString(module, "translate");
        if (!translate) {
            tryLsstExceptionWarn("Failed to find translation function for C++ Exceptions.");
            failed = true;
        } else {
            PyObject* instance = PyObject_CallFunctionObjArgs(translate, pyex, NULL);
            if (!instance) {
                // we actually expect a null return here, as translate() should raise an exception
                tryLsstExceptionWarn("Failed to translate C++ Exception to Python.");
                failed = true;
            } else {
                PyObject* type = PyObject_Type(instance);
                PyErr_SetObject(type, instance);
                Py_DECREF(type);
                Py_DECREF(instance);
            }
            Py_DECREF(translate);
        }
        Py_DECREF(module);
    }

    Py_XDECREF(pyex);

    if (failed) {
        PyErr_SetString(PyExc_RuntimeError, const_cast<char*>(ex.what()));
    }
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
