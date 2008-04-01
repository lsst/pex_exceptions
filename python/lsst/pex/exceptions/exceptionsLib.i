// -*- lsst-c++ -*-
%define pex_exceptions_DOCSTRING
"
Access to the classes from the pex_exceptions library
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.pex.exceptions", docstring=pex_exceptions_DOCSTRING) exceptionsLib


/******************************************************************************/
/*
 * Mapping C++ exceptions to Python
 */

%init %{
%}

%pythoncode %{
    import lsst.utils
%}

%include "lsst/p_lsstSwig.i"

%{
#include <new>
#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"
%}

%inline %{
namespace lsst { namespace pex { namespace exceptions { } } }
%}

// Use the Python C API to create the constructor argument tuple (a message string and a
// DataProperty corresponding to an ExceptionStack) for a Python exception class assumed to
// be derived from lsst.pex.exceptions.LsstExceptionStack. Also obtain a class object for
// the desired exception type. Use the class object and tuple to raise a Python exception.
%{
static void raiseLsstExceptionStack(lsst::pex::exceptions::ExceptionStack & ex) {
    PyObject * modules = PyImport_GetModuleDict();
    PyObject * module  = PyDict_GetItemString(modules, ex.getPythonModule());
    if (module == 0) {
        PyErr_Format(PyExc_ImportError, "failed to find LSST exception module '%s'", ex.getPythonModule());
        return;
    }
    PyObject * clazz  = PyDict_GetItem(PyModule_GetDict(module), PyString_FromString(ex.getPythonClass()));
    if (clazz == 0) {
        PyErr_Format(PyExc_AttributeError, "unable to find LSST exception class '%s' in module '%s'",
                     ex.getPythonClass(), ex.getPythonModule());
        return;
    }

    PyObject * args = PyTuple_New(2);
    if (args == 0) {
        PyErr_SetString(clazz, ex.what());
        return;
    }

    PyTuple_SetItem(args, 0, PyString_FromString(ex.what()));

    PyObject       * stack = 0;
    swig_type_info * tinfo = SWIG_TypeQuery("boost::shared_ptr<lsst::daf::base::DataProperty> *");
    if (tinfo != 0) {
        void * ptr = static_cast<void *>(new lsst::daf::base::DataProperty::PtrType(ex.getStack()));
        stack = SWIG_NewPointerObj(static_cast<void *>(ptr), tinfo, SWIG_POINTER_OWN);
    } else {
        stack = Py_None;
        Py_INCREF(stack);
    }
    PyTuple_SetItem(args, 1, stack);

    PyErr_SetObject(clazz, args);
    Py_DECREF(args);
}
%}

// Specifies the default C++ to python exception handling interface
%exception {
    try {
        $action
    } catch (lsst::pex::exceptions::ExceptionStack &e) {
        raiseLsstExceptionStack(e);
        SWIG_fail;
    } catch (std::exception & e) {
        PyErr_SetString(PyExc_Exception, e.what());
        SWIG_fail;
    }
}

%pythoncode %{
    import lsst.pex.exceptions
%}


/******************************************************************************/
// Local Variables: ***
// eval: (setq indent-tabs-mode nil) ***
// End: ***
