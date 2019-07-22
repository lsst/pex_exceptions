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

#include "lsst/pex/exceptions/Exception.h"

#define BOOST_TEST_MODULE Exception_1
#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"
#include "boost/format.hpp"

namespace test = boost::test_tools;
namespace pexExcept = lsst::pex::exceptions;

#include "test_Exception_1.h"

// These functions were created to avoid having the output strings, which
// include function names, depend on the boost::test implementation.  Note
// that the output strings also depend on the name of this file and line
// numbers within it, however.

void f1(void) { throw LSST_EXCEPT(pexExcept::Exception, "In f1"); }

void f2(void) { throw LSST_EXCEPT(ChildException, (boost::format("In f2 %1%") % 2008).str()); }

void f4(void) {
    try {
        f1();
    } catch (pexExcept::Exception& e) {
        LSST_EXCEPT_ADD(e, "In f4");
        throw;
    }
}

void f5(void) {
    try {
        f2();
    } catch (pexExcept::Exception& e) {
        LSST_EXCEPT_ADD(e, "In f5");
        throw;
    }
}

void f6(void) {
    try {
        f2();
    } catch (ChildException& e) {
        LSST_EXCEPT_ADD(e, "In f6");
        throw;
    }
}

void f7(void) {
    try {
        f6();
    } catch (ChildException& e) {
        LSST_EXCEPT_ADD(e, "In f7");
        throw;
    }
}

BOOST_AUTO_TEST_SUITE(ExceptionSuite)

BOOST_AUTO_TEST_CASE(base) {
    BOOST_CHECK_THROW(f1(), pexExcept::Exception);
    BOOST_CHECK_THROW(f2(), ChildException);
    BOOST_CHECK_THROW(f4(), pexExcept::Exception);
    BOOST_CHECK_THROW(f5(), ChildException);
    BOOST_CHECK_THROW(f6(), ChildException);
    BOOST_CHECK_THROW(f7(), ChildException);
}

BOOST_AUTO_TEST_CASE(simple) {
    test::output_test_stream o;
    try {
        f1();
    } catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 42, in void f1()\n"
                       "    In f1 {0}\n"
                       "lsst::pex::exceptions::Exception: 'In f1'\n"));
}

BOOST_AUTO_TEST_CASE(child_as_base) {
    test::output_test_stream o;
    try {
        f2();
    } catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 44, in void f2()\n"
                       "    In f2 2008 {0}\n"
                       "ChildException: 'In f2 2008'\n"));
}

BOOST_AUTO_TEST_CASE(child_as_child) {
    test::output_test_stream o;
    try {
        f2();
    } catch (ChildException const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 44, in void f2()\n"
                       "    In f2 2008 {0}\n"
                       "ChildException: 'In f2 2008'\n"));
}

BOOST_AUTO_TEST_CASE(simple_rethrow) {
    test::output_test_stream o;
    try {
        f4();
    } catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 42, in void f1()\n"
                       "    In f1 {0}\n"
                       "  File \"tests/test_Exception_1.cc\", line 50, in void f4()\n"
                       "    In f4 {1}\n"
                       "lsst::pex::exceptions::Exception: 'In f1 {0}; In f4 {1}'\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_base_as_base) {
    test::output_test_stream o;
    try {
        f5();
    } catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 44, in void f2()\n"
                       "    In f2 2008 {0}\n"
                       "  File \"tests/test_Exception_1.cc\", line 59, in void f5()\n"
                       "    In f5 {1}\n"
                       "ChildException: 'In f2 2008 {0}; In f5 {1}'\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_base_as_child) {
    test::output_test_stream o;
    try {
        f5();
    } catch (ChildException const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 44, in void f2()\n"
                       "    In f2 2008 {0}\n"
                       "  File \"tests/test_Exception_1.cc\", line 59, in void f5()\n"
                       "    In f5 {1}\n"
                       "ChildException: 'In f2 2008 {0}; In f5 {1}'\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_child_as_base) {
    test::output_test_stream o;
    try {
        f6();
    } catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 44, in void f2()\n"
                       "    In f2 2008 {0}\n"
                       "  File \"tests/test_Exception_1.cc\", line 68, in void f6()\n"
                       "    In f6 {1}\n"
                       "ChildException: 'In f2 2008 {0}; In f6 {1}'\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_child_as_child) {
    test::output_test_stream o;
    try {
        f6();
    } catch (ChildException const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 44, in void f2()\n"
                       "    In f2 2008 {0}\n"
                       "  File \"tests/test_Exception_1.cc\", line 68, in void f6()\n"
                       "    In f6 {1}\n"
                       "ChildException: 'In f2 2008 {0}; In f6 {1}'\n"));
}

BOOST_AUTO_TEST_CASE(throw_without_macro) {
    test::output_test_stream o;
    try {  // this form shouldn't be used, but we don't want things to explode if it is
        throw ChildException("python-only ctor");
    } catch (ChildException& e) {
        LSST_EXCEPT_ADD(e, "new message");
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal("python-only ctor; new message"));
}

BOOST_AUTO_TEST_CASE(rethrow_twice) {
    test::output_test_stream o;
    try {
        f7();
    } catch (ChildException const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(
            o.is_equal("\n"
                       "  File \"tests/test_Exception_1.cc\", line 44, in void f2()\n"
                       "    In f2 2008 {0}\n"
                       "  File \"tests/test_Exception_1.cc\", line 68, in void f6()\n"
                       "    In f6 {1}\n"
                       "  File \"tests/test_Exception_1.cc\", line 77, in void f7()\n"
                       "    In f7 {2}\n"
                       "ChildException: 'In f2 2008 {0}; In f6 {1}; In f7 {2}'\n"));
}

BOOST_AUTO_TEST_SUITE_END()
