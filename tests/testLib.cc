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

#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "lsst/pex/exceptions/Exception.h"
#include "lsst/pex/exceptions/Runtime.h"
#include "lsst/pex/exceptions/python/Exception.h"

using namespace lsst::pex::exceptions;

LSST_EXCEPTION_TYPE(TestError, lsst::pex::exceptions::RuntimeError, TestError)

template <typename T>
void fail1(std::string const &message) {
    throw LSST_EXCEPT(T, message);
}

template <typename T>
void fail2(std::string const &message1, std::string const &message2) {
    try {
        fail1<T>(message1);
    } catch (T &err) {
        LSST_EXCEPT_ADD(err, message2);
        throw err;
    }
}

#define LSST_FAIL_TEST(name)                                                                 \
    mod.def("fail" #name "1", [](const std::string &message) { fail1<name>(message); });     \
    mod.def("fail" #name "2", [](const std::string &message1, const std::string &message2) { \
        fail2<name>(message1, message2);                                                     \
    });

NB_MODULE(_testLib, mod) {
    auto cls = python::declareException<TestError>(mod, "TestError", "RuntimeError");
    cls.def(nanobind::init<std::string const &>());

    LSST_FAIL_TEST(TestError)
    LSST_FAIL_TEST(IoError)
    LSST_FAIL_TEST(OverflowError)
    LSST_FAIL_TEST(RangeError)
    LSST_FAIL_TEST(TypeError)
    LSST_FAIL_TEST(UnderflowError)
    LSST_FAIL_TEST(DomainError)
    LSST_FAIL_TEST(InvalidParameterError)
    LSST_FAIL_TEST(LengthError)
    LSST_FAIL_TEST(OutOfRangeError)
    LSST_FAIL_TEST(LogicError)
    LSST_FAIL_TEST(NotFoundError)
    LSST_FAIL_TEST(RuntimeError)
    LSST_FAIL_TEST(Exception)
}
