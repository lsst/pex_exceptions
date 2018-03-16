#include "pybind11/pybind11.h"

#include <sstream>

#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"

using namespace lsst::pex::exceptions;

namespace py = pybind11;

namespace lsst {
namespace pex {
namespace exceptions {
namespace {
// Helper function to show a Python warning when exception translation fails.
void tryLsstExceptionWarn(const char *message) {
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
void raiseLsstException(py::object &pyex) {
    static auto module =
            py::reinterpret_borrow<py::object>(PyImport_ImportModule("lsst.pex.exceptions.wrappers"));
    if (!module.ptr()) {
        tryLsstExceptionWarn("Failed to import C++ Exception wrapper module.");
    } else {
        static auto translate =
                py::reinterpret_borrow<py::object>(PyObject_GetAttrString(module.ptr(), "translate"));
        if (!translate.ptr()) {
            tryLsstExceptionWarn("Failed to find translation function for C++ Exceptions.");
        } else {
            // Calling the Python translate() returns an instance of the appropriate Python
            // exception that wraps the C++ exception instance that we give it.
            auto instance = py::reinterpret_steal<py::object>(
                    PyObject_CallFunctionObjArgs(translate.ptr(), pyex.ptr(), NULL));
            if (!instance.ptr()) {
                // We actually expect a null return here, as translate() should raise an exception
                tryLsstExceptionWarn("Failed to translate C++ Exception to Python.");
            } else {
                auto type = py::reinterpret_borrow<py::object>(PyObject_Type(instance.ptr()));
                PyErr_SetObject(type.ptr(), instance.ptr());
            }
        }
    }
}
}  // <anonymous>

PYBIND11_PLUGIN(exceptions) {
    py::module mod("exceptions");

    py::class_<Tracepoint> clsTracepoint(mod, "Tracepoint");

    clsTracepoint.def(py::init<char const *, int, char const *, std::string const &>())
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
            .def("asString",
                 [](Exception &self) -> std::string {
                     std::ostringstream stream;
                     self.addToStream(stream);
                     return stream.str();
                 })
            .def("__repr__", [](Exception &self) -> std::string {
                std::stringstream s;
                s << "Exception('" << self.what() << "')";
                return s.str();
            });

    py::class_<LogicError, Exception> clsLogicError(mod, "LogicError");
    clsLogicError.def(py::init<std::string const &>());

    py::class_<NotFoundError, Exception> clsNotFoundError(mod, "NotFoundError");
    clsNotFoundError.def(py::init<std::string const &>());

    py::class_<RuntimeError, Exception> clsRuntimeError(mod, "RuntimeError");
    clsRuntimeError.def(py::init<std::string const &>());

    py::class_<IoError, RuntimeError> clsIoError(mod, "IoError");
    clsIoError.def(py::init<std::string const &>());

    py::class_<OverflowError, RuntimeError> clsOverflowError(mod, "OverflowError");
    clsOverflowError.def(py::init<std::string const &>());

    py::class_<RangeError, RuntimeError> clsRangeError(mod, "RangeError");
    clsRangeError.def(py::init<std::string const &>());

    py::class_<TypeError, RuntimeError> clsTypeError(mod, "TypeError");
    clsTypeError.def(py::init<std::string const &>());

    py::class_<UnderflowError, RuntimeError> clsUnderflowError(mod, "UnderflowError");
    clsUnderflowError.def(py::init<std::string const &>());

    py::class_<DomainError, LogicError> clsDomainError(mod, "DomainError");
    clsDomainError.def(py::init<std::string const &>());

    py::class_<InvalidParameterError, LogicError> clsInvalidParameterError(mod, "InvalidParameterError");
    clsInvalidParameterError.def(py::init<std::string const &>());

    py::class_<LengthError, LogicError> clsLengthError(mod, "LengthError");
    clsLengthError.def(py::init<std::string const &>());

    py::class_<OutOfRangeError, LogicError> clsOutOfRangeError(mod, "OutOfRangeError");
    clsOutOfRangeError.def(py::init<std::string const &>());

    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const Exception &e) {
            py::object current_exception;
            current_exception = py::cast(e.clone(), py::return_value_policy::take_ownership);
            raiseLsstException(current_exception);
        }
    });

    return mod.ptr();
}

}  // exceptions
}  // pex
}  // lsst
