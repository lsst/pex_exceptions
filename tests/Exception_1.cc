#include "lsst/pex/exceptions/Exception.h"

#define BOOST_TEST_MODULE Exception_1
#include "boost/test/included/unit_test.hpp"
#include "boost/test/output_test_stream.hpp"

#include "boost/format.hpp"

namespace test = boost::test_tools;
namespace pexExcept = lsst::pex::exceptions;

#include "Exception_1.h"

// class DetailedException : public pexExcept::Exception {
// public:
//     DetailedException(LSST_EARGS_TYPED, int count) :
//         Exception(LSST_EARGS_UNTYPED), _count(count) { };
//     virtual std::ostream& addToStream(std::ostream& stream) const {
//         // Booby-trapped to demonstrate what() exception handling.
//         if (_count > 9) throw std::exception();
//         // Demonstrates how to add information before and after base class.
//         return pexExcept::Exception::addToStream(
//             stream << "Begin DetailedException(" << _count <<
//             ")" << std::endl) <<
//             "End DetailedException" << std::endl;
//     };
//     int getCount(void) const { return _count; };
// private:
//     int _count;
// };


// These functions were created to avoid having the output strings, which
// include function names, depend on the boost::test implementation.  Note
// that the output strings also depend on the name of this file and line
// numbers within it, however.

void f1(void) {
    throw LSST_EXCEPT(pexExcept::Exception, "In f1");
}

void f2(void) {
    throw LSST_EXCEPT(ChildException, (boost::format("In f2 %1%") % 2008).str());
}

void f3(int i) {
    throw LSST_EXCEPT(DetailedException, (boost::format("In f3 %1%") % "foo").str(), i);
}

void f4(void) {
    try {
        f1();
    }
    catch (pexExcept::Exception& e) {
        LSST_EXCEPT_ADD_HERE(e);
        throw;
    }
}

void f5(void) {
    try {
        f1();
    }
    catch (pexExcept::Exception& e) {
        LSST_EXCEPT_ADD_MESSAGE(e, "In f5");
        throw;
    }
}

void f6(void) {
    try {
        f2();
    }
    catch (pexExcept::Exception& e) {
        LSST_EXCEPT_ADD_HERE(e);
        throw;
    }
}

void f7(void) {
    try {
        f2();
    }
    catch (pexExcept::Exception& e) {
        LSST_EXCEPT_ADD_MESSAGE(e, "In f7");
        throw;
    }
}

void f8(void) {
    try {
        f2();
    }
    catch (ChildException& e) {
        LSST_EXCEPT_ADD_HERE(e);
        throw;
    }
}

void f9(void) {
    try {
        f2();
    }
    catch (ChildException& e) {
        LSST_EXCEPT_ADD_MESSAGE(e, "In f9");
        throw;
    }
}

void f10(int i) {
    try {
        f3(i);
    }
    catch (pexExcept::Exception& e) {
        LSST_EXCEPT_ADD_MESSAGE(e, (boost::format("In f10 (%1% %2% %3%)") % i % 42 % 3.14159).str());
        throw;
    }
}

void f11(int i) {
    try {
        f3(i);
    }
    catch (DetailedException& e) {
        LSST_EXCEPT_ADD_MESSAGE(e, "In f11");
        throw;
    }
}

void f12(int i) {
    try {
        f3(i);
    }
    catch (ChildException& e) {
        // Should not be executed
        LSST_EXCEPT_ADD_MESSAGE(e, "In f12");
        throw;
    }
}

BOOST_AUTO_TEST_SUITE(ExceptionSuite)

BOOST_AUTO_TEST_CASE(base) {
    BOOST_CHECK_THROW(f1(), pexExcept::Exception);
    BOOST_CHECK_THROW(f2(), ChildException);
    BOOST_CHECK_THROW(f3(1), DetailedException);
    BOOST_CHECK_THROW(f4(), pexExcept::Exception);
    BOOST_CHECK_THROW(f5(), pexExcept::Exception);
    BOOST_CHECK_THROW(f6(), pexExcept::Exception);
    BOOST_CHECK_THROW(f7(), ChildException);
    BOOST_CHECK_THROW(f8(), ChildException);
    BOOST_CHECK_THROW(f9(), ChildException);
    BOOST_CHECK_THROW(f10(2), DetailedException);
    BOOST_CHECK_THROW(f11(3), DetailedException);
    BOOST_CHECK_THROW(f12(4), DetailedException);
}

BOOST_AUTO_TEST_CASE(simple) {
    test::output_test_stream o;
    try {
        f1();
    }
    catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: pexExcept::Exception thrown at tests/Exception_1.cc:39 in void f1()\n"
            "0: Message: In f1\n"));
}

BOOST_AUTO_TEST_CASE(child_as_base) {
    test::output_test_stream o;
    try {
        f2();
    }
    catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: ChildException thrown at tests/Exception_1.cc:43 in void f2()\n"
            "0: Message: In f2 2008\n"));
}

BOOST_AUTO_TEST_CASE(child_as_child) {
    test::output_test_stream o;
    try {
        f2();
    }
    catch (ChildException const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: ChildException thrown at tests/Exception_1.cc:43 in void f2()\n"
            "0: Message: In f2 2008\n"));
}

BOOST_AUTO_TEST_CASE(detailed_as_detailed) {
    test::output_test_stream o;
    try {
        f3(5);
    }
    catch (DetailedException const& e) {
        BOOST_CHECK_EQUAL(e.getCount(), 5);
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "Begin DetailedException(5)\n"
            "0: DetailedException thrown at tests/Exception_1.cc:47 in void f3(int)\n"
            "0: Message: In f3 foo\n"
            "End DetailedException\n"));
}

BOOST_AUTO_TEST_CASE(simple_rethrow_here) {
    test::output_test_stream o;
    try {
        f4();
    }
    catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: pexExcept::Exception thrown at tests/Exception_1.cc:39 in void f1()\n"
            "0: Message: In f1\n"
            "1: Rethrown at tests/Exception_1.cc:55 in void f4()\n"));
}

BOOST_AUTO_TEST_CASE(simple_rethrow_message) {
    test::output_test_stream o;
    try {
        f5();
    }
    catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: pexExcept::Exception thrown at tests/Exception_1.cc:39 in void f1()\n"
            "0: Message: In f1\n"
            "1: Rethrown at tests/Exception_1.cc:65 in void f5()\n"
            "1: Message: In f5\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_base_with_here_as_base) {
    test::output_test_stream o;
    try {
        f6();
    }
    catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: ChildException thrown at tests/Exception_1.cc:43 in void f2()\n"
            "0: Message: In f2 2008\n"
            "1: Rethrown at tests/Exception_1.cc:75 in void f6()\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_base_with_message_as_child) {
    test::output_test_stream o;
    try {
        f7();
    }
    catch (ChildException const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: ChildException thrown at tests/Exception_1.cc:43 in void f2()\n"
            "0: Message: In f2 2008\n"
            "1: Rethrown at tests/Exception_1.cc:85 in void f7()\n"
            "1: Message: In f7\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_child_with_here_as_child) {
    test::output_test_stream o;
    try {
        f8();
    }
    catch (ChildException const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: ChildException thrown at tests/Exception_1.cc:43 in void f2()\n"
            "0: Message: In f2 2008\n"
            "1: Rethrown at tests/Exception_1.cc:95 in void f8()\n"));
}

BOOST_AUTO_TEST_CASE(child_rethrow_child_with_message_as_base) {
    test::output_test_stream o;
    try {
        f9();
    }
    catch (pexExcept::Exception const& e) {
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "0: ChildException thrown at tests/Exception_1.cc:43 in void f2()\n"
            "0: Message: In f2 2008\n"
            "1: Rethrown at tests/Exception_1.cc:105 in void f9()\n"
            "1: Message: In f9\n"));
}

BOOST_AUTO_TEST_CASE(detailed_rethrow_base_as_detailed) {
    test::output_test_stream o;
    try {
        f10(6);
    }
    catch (DetailedException const& e) {
        BOOST_CHECK_EQUAL(e.getCount(), 6);
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "Begin DetailedException(6)\n"
            "0: DetailedException thrown at tests/Exception_1.cc:47 in void f3(int)\n"
            "0: Message: In f3 foo\n"
            "1: Rethrown at tests/Exception_1.cc:115 in void f10(int)\n"
            "1: Message: In f10 (6 42 3.14159)\n"
            "End DetailedException\n"));
}

BOOST_AUTO_TEST_CASE(detailed_rethrow_detailed_as_detailed) {
    test::output_test_stream o;
    try {
        f11(7);
    }
    catch (DetailedException const& e) {
        BOOST_CHECK_EQUAL(e.getCount(), 7);
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "Begin DetailedException(7)\n"
            "0: DetailedException thrown at tests/Exception_1.cc:47 in void f3(int)\n"
            "0: Message: In f3 foo\n"
            "1: Rethrown at tests/Exception_1.cc:125 in void f11(int)\n"
            "1: Message: In f11\n"
            "End DetailedException\n"));
}

BOOST_AUTO_TEST_CASE(detailed_rethrow_child_as_detailed) {
    test::output_test_stream o;
    try {
        f12(8);
    }
    catch (DetailedException const& e) {
        BOOST_CHECK_EQUAL(e.getCount(), 8);
        o << e;
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "Begin DetailedException(8)\n"
            "0: DetailedException thrown at tests/Exception_1.cc:47 in void f3(int)\n"
            "0: Message: In f3 foo\n"
            "End DetailedException\n"));
}

BOOST_AUTO_TEST_CASE(detailed_what_normal) {
    test::output_test_stream o;
    try {
        f12(9);
    }
    catch (DetailedException const& e) {
        BOOST_CHECK_EQUAL(e.getCount(), 9);
        o << e.what();
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(
            "Begin DetailedException(9)\n"
            "0: DetailedException thrown at tests/Exception_1.cc:47 in void f3(int)\n"
            "0: Message: In f3 foo\n"
            "End DetailedException\n"));
}

BOOST_AUTO_TEST_CASE(detailed_what_boobytrap) {
    test::output_test_stream o;
    try {
        f12(10);
    }
    catch (DetailedException const& e) {
        BOOST_CHECK_EQUAL(e.getCount(), 10);
        o << e.what();
    }
    BOOST_CHECK(!o.is_empty(false));
    BOOST_CHECK(o.is_equal(typeid(DetailedException).name()));
}

BOOST_AUTO_TEST_SUITE_END()
