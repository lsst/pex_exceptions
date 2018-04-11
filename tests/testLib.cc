#include <string>

#include <pybind11/pybind11.h>

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

PYBIND11_PLUGIN(_testLib) {
    pybind11::module mod("_testLib", "Tests for the pex_exceptions library");

    auto cls = python::declareException<TestError>(mod, "TestError", "RuntimeError");
    cls.def(pybind11::init<std::string const &>());

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

    return mod.ptr();
}