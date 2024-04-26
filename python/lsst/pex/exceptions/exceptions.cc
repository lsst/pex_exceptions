/*
 * This file is part of pex_exceptions.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <sstream>

#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"

using namespace lsst::pex::exceptions;

namespace nb = nanobind;

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

/**
 * Raise a Python exception that wraps the given C++ exception instance.
 *
 * Most of the work is delegated to the pure-Python function pex.exceptions.wrappers.translate(),
 * which looks up the appropriate Python exception class from a dict that maps C++ exception
 * types to their custom Python wrappers.  Everything else here is basically just importing that
 * module, preparing the arguments, and calling that function, along with the very verbose error
 * handling required by the Python C API.
 *
 * If any point we fail to translate the exception, we print a Python warning and raise the built-in
 * Python RuntimeError exception with the same message as the C++ exception.
 *
 * @param pyex a wrapped instance of pex::exceptions::Exception
 */
void raiseLsstException(nb::object &pyex) {
    static auto module =
            nb::borrow<nb::object>(PyImport_ImportModule("lsst.pex.exceptions.wrappers"));
    if (!module.ptr()) {
        tryLsstExceptionWarn("Failed to import C++ Exception wrapper module.");
    } else {
        static auto translate =
                nb::borrow<nb::object>(PyObject_GetAttrString(module.ptr(), "translate"));
        if (!translate.ptr()) {
            tryLsstExceptionWarn("Failed to find translation function for C++ Exceptions.");
        } else {
            // Calling the Python translate() returns an instance of the appropriate Python
            // exception that wraps the C++ exception instance that we give it.
            auto instance = nb::steal<nb::object>(
                    PyObject_CallFunctionObjArgs(translate.ptr(), pyex.ptr(), NULL));
            if (!instance.ptr()) {
                // We actually expect a null return here, as translate() should raise an exception
                tryLsstExceptionWarn("Failed to translate C++ Exception to Python.");
            } else {
                auto type = nb::borrow<nb::object>(PyObject_Type(instance.ptr()));
                PyErr_SetObject(type.ptr(), instance.ptr());
            }
        }
    }
}
}  // namespace

NB_MODULE(exceptions, mod) {
    nb::class_<Tracepoint> clsTracepoint(mod, "Tracepoint");

    clsTracepoint.def(nb::init<char const *, int, char const *, std::string const &>())
            .def_rw("_file", &Tracepoint::_file)
            .def_rw("_line", &Tracepoint::_line)
            .def_rw("_func", &Tracepoint::_func)
            .def_rw("_message", &Tracepoint::_message);

    nb::class_<Exception> clsException(mod, "Exception");

    clsException.def(nb::init<std::string const &>())
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

    nb::class_<LogicError, Exception> clsLogicError(mod, "LogicError");
    clsLogicError.def(nb::init<std::string const &>());

    nb::class_<NotFoundError, Exception> clsNotFoundError(mod, "NotFoundError");
    clsNotFoundError.def(nb::init<std::string const &>());

    nb::class_<RuntimeError, Exception> clsRuntimeError(mod, "RuntimeError");
    clsRuntimeError.def(nb::init<std::string const &>());

    nb::class_<IoError, RuntimeError> clsIoError(mod, "IoError");
    clsIoError.def(nb::init<std::string const &>());

    nb::class_<OverflowError, RuntimeError> clsOverflowError(mod, "OverflowError");
    clsOverflowError.def(nb::init<std::string const &>());

    nb::class_<RangeError, RuntimeError> clsRangeError(mod, "RangeError");
    clsRangeError.def(nb::init<std::string const &>());

    nb::class_<TypeError, LogicError> clsTypeError(mod, "TypeError");
    clsTypeError.def(nb::init<std::string const &>());

    nb::class_<UnderflowError, RuntimeError> clsUnderflowError(mod, "UnderflowError");
    clsUnderflowError.def(nb::init<std::string const &>());

    nb::class_<DomainError, LogicError> clsDomainError(mod, "DomainError");
    clsDomainError.def(nb::init<std::string const &>());

    nb::class_<InvalidParameterError, LogicError> clsInvalidParameterError(mod, "InvalidParameterError");
    clsInvalidParameterError.def(nb::init<std::string const &>());

    nb::class_<LengthError, LogicError> clsLengthError(mod, "LengthError");
    clsLengthError.def(nb::init<std::string const &>());

    nb::class_<OutOfRangeError, LogicError> clsOutOfRangeError(mod, "OutOfRangeError");
    clsOutOfRangeError.def(nb::init<std::string const &>());

    nb::register_exception_translator([](const std::exception_ptr &p, void *) {
        try {
            std::rethrow_exception(p);
        } catch (const Exception &e) {
            nb::object current_exception;
            current_exception = nb::cast(e.clone(), nb::rv_policy::take_ownership);
            raiseLsstException(current_exception);
        }
    });
}

}  // namespace exceptions
}  // namespace pex
}  // namespace lsst
