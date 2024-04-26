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

#ifndef LSST_PEX_EXCEPTIONS_PYTHON_EXCEPTION_H
#define LSST_PEX_EXCEPTIONS_PYTHON_EXCEPTION_H

#include <string>

#include <nanobind/nanobind.h>

#include "lsst/pex/exceptions/Exception.h"

namespace lsst {
namespace pex {
namespace exceptions {
namespace python {

/**
 * Helper function for nanobind, used to define new types of exceptions.
 *
 * While this function creates the class wrapper, the user is still responsible
 * for adding all constructor and member wrappers to the returned `nb::class_` object.
 *
 * @tparam T The C++ exception to wrap.
 * @tparam E The C++ base class of `T`.
 *
 * @param[in] mod Module to insert the exception into.
 * @param[in] name Name of the exception in the module.
 * @param[in] base Python name of base class (from pex::exceptions).
 */
template <typename T, typename E = lsst::pex::exceptions::Exception>
nanobind::class_<T, E> declareException(nanobind::module_ &mod, const std::string &name,
                                     const std::string &base) {
    namespace nb = nanobind;

    // Wrap T as a new Python exception type with *name* and add it to the module
    //
    // Note that all created C++ wrapped type derive from Exception here.
    // It is only in the pure Python wrapper layer that they get embedded in
    // a subclass of the requested base.
   nb::class_<T, E> cls(mod, name.c_str());

    // Declare T wrapped by cls as a pex exception and register it
    // this relies on the pure Python function "declare" defined in "wrappers"
    // to create a corresponding Python type derived from Python standard Exception
    auto exceptions =
           nb::steal<nb::object>(PyImport_ImportModule("lsst.pex.exceptions.wrappers"));
    if (!exceptions.ptr()) {
        throw nb::import_error("import failed");
    }

    auto declare =nb::steal<nb::object>(PyObject_GetAttrString(exceptions.ptr(), "declare"));
    if (!declare.ptr()) {
        throw nb::attribute_error("could not get declare function from Python");
    }

    auto baseCls =nb::steal<nb::object>(PyObject_GetAttrString(exceptions.ptr(), base.c_str()));
    if (!baseCls.ptr()) {
        throw nb::attribute_error("could not get base class");
    }

    auto exceptionName =nb::steal<nb::object>(PyUnicode_FromString(name.c_str()));
    if (!exceptionName.ptr()) {
        throw std::string("could not create name string");
    }

    auto result =nb::steal<nb::object>(PyObject_CallFunctionObjArgs(
            declare.ptr(), mod.ptr(), exceptionName.ptr(), baseCls.ptr(), cls.ptr(), NULL));
    if (!result.ptr()) {
        PyErr_SetString(PyExc_SystemError, "could not declare exception");
        throw std::string("could not declare exception");
    }

    return cls;
}

}  // namespace python
}  // namespace exceptions
}  // namespace pex
}  // namespace lsst

#endif
