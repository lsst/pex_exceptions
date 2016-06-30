#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/exception_translator.h>

#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"

using namespace lsst::pex::exceptions;

namespace py = pybind11;

// Helper function to show a Python warning when exception translation fails.
static void tryLsstExceptionWarn(const char * message) {
    // Try to warn that exception translation failed, if we fail, clear the exception raised by the
    // warning attempt so we can raise a less-informative exception based on the original.
    int s = PyErr_WarnEx(PyExc_Warning, message, 1);
    if (s) {
        PyErr_Clear();
    }
}

// Raise a Python exception that wraps the given C++ exception instance.
//
// Most of the work is delegated to the pure-Python function pex.exceptions.wrappers.translate(),
// which looks up the appropriate Python exception class from a dict that maps C++ exception
// types to their custom Python wrappers.  Everything else here is basically just importing that
// module, preparing the arguments, and calling that function, along with the very verbose error
// handling required by the Python C API.
//
// If any point we fail to translate the exception, we print a Python warning and raise the built-in
// Python RuntimeError exception with the same message as the C++ exception.
static void raiseLsstException(py::object &pyex) {
    static py::object module{PyImport_ImportModule("lsst.pex.exceptions.wrappers"), false};
    if (!module.ptr()) {
        tryLsstExceptionWarn("Failed to import C++ Exception wrapper module.");
    } else {
        static py::object translate{PyObject_GetAttrString(module.ptr(), "translate"), false};
        if (!translate.ptr()) {
            tryLsstExceptionWarn("Failed to find translation function for C++ Exceptions.");
        } else {
            // Calling the Python translate() returns an instance of the appropriate Python
            // exception that wraps the C++ exception instance that we give it.
            py::object instance{PyObject_CallFunctionObjArgs(translate.ptr(), pyex.ptr(), NULL),
                false};
            if (!instance.ptr()) {
                // We actually expect a null return here, as translate() should raise an exception
                tryLsstExceptionWarn("Failed to translate C++ Exception to Python.");
            } else {
                py::object type{PyObject_Type(instance.ptr()), false};
                PyErr_SetObject(type.ptr(), instance.ptr());
            }
        }
    }
}

PYBIND11_PLUGIN(_exceptions) {
    py::module mod("_exceptions", "Access to the classes from the pex_exceptions library");

    py::class_<Tracepoint> clsTracepoint(mod, "Tracepoint");

    clsTracepoint.def(py::init<char const*, int, char const*, std::string const &>())
        .def_readwrite("_file", &Tracepoint::_file)
        .def_readwrite("_line", &Tracepoint::_line)
        .def_readwrite("_func", &Tracepoint::_func)
        .def_readwrite("_message", &Tracepoint::_message);

    py::class_<Exception> clsException(mod, "Exception");

    clsException.def(py::init<std::string const &>())
        .def("addMessage", &Exception::addMessage)
        .def("getTraceback", &Exception::getTraceback)
        .def("addToStream", &Exception::addToStream)
        .def("what", &Exception::what)
        .def("getType", &Exception::getType)
        .def("clone", &Exception::clone)
        .def("asString", [](Exception &e) -> std::string {
            std::ostringstream stream;
            e.addToStream(stream);
            return stream.str();
        })
        .def("__repr__", [](Exception &e) -> std::string {
                std::stringstream s;
                s<<"Exception('"<<e.what()<<"')";
                return s.str();
        });

    py::class_<LogicError> clsLogicError(mod, "LogicError", py::base<Exception>()); 
    clsLogicError.def(py::init<std::string const &>());

    py::class_<NotFoundError> clsNotFoundError(mod, "NotFoundError", py::base<Exception>()); 
    clsNotFoundError.def(py::init<std::string const &>());

    py::class_<RuntimeError> clsRuntimeError(mod, "RuntimeError", py::base<Exception>()); 
    clsRuntimeError.def(py::init<std::string const &>());

    py::class_<IoError> clsIoError(mod, "IoError", py::base<RuntimeError>()); 
    clsIoError.def(py::init<std::string const &>());

    py::class_<MemoryError> clsMemoryError(mod, "MemoryError", py::base<RuntimeError>()); 
    clsMemoryError.def(py::init<std::string const &>());

    py::class_<OverflowError> clsOverflowError(mod, "OverflowError", py::base<RuntimeError>()); 
    clsOverflowError.def(py::init<std::string const &>());

    py::class_<RangeError> clsRangeError(mod, "RangeError", py::base<RuntimeError>()); 
    clsRangeError.def(py::init<std::string const &>());

    py::class_<TimeoutError> clsTimeoutError(mod, "TimeoutError", py::base<RuntimeError>()); 
    clsTimeoutError.def(py::init<std::string const &>());

    py::class_<TypeError> clsTypeError(mod, "TypeError", py::base<RuntimeError>()); 
    clsTypeError.def(py::init<std::string const &>());

    py::class_<UnderflowError> clsUnderflowError(mod, "UnderflowError", py::base<RuntimeError>()); 
    clsUnderflowError.def(py::init<std::string const &>());

    py::class_<DomainError> clsDomainError(mod, "DomainError", py::base<LogicError>()); 
    clsDomainError.def(py::init<std::string const &>());

    py::class_<InvalidParameterError> clsInvalidParameterError(mod, "InvalidParameterError", py::base<LogicError>()); 
    clsInvalidParameterError.def(py::init<std::string const &>());

    py::class_<LengthError> clsLengthError(mod, "LengthError", py::base<LogicError>()); 
    clsLengthError.def(py::init<std::string const &>());

    py::class_<OutOfRangeError> clsOutOfRangeError(mod, "OutOfRangeError", py::base<LogicError>()); 
    clsOutOfRangeError.def(py::init<std::string const &>());

    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const Exception &e) {
            py::object current_exception;
            current_exception = py::cast(e);
            raiseLsstException(current_exception);
        }
    });

    return mod.ptr();
}