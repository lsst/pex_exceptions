// -*- lsst-c++ -*-
#include "lsst/pex/exceptions/python.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#define DECLARE_LOCAL_SUBCLASS(NAME, BASE)                              \
    lsst::pex::exceptions::PyException::subclass< lsst::pex::exceptions::NAME, \
                                                  lsst::pex::exceptions::BASE >(#NAME)

namespace bp = boost::python;

namespace lsst { namespace pex { namespace exceptions {

bool operator==(Tracepoint const & a, Tracepoint const & b) {
    return a._file == b._file && a._line == b._line && a._func == b._func && a._msg == b._msg;
}

void PyException::translate(Exception const & err) {
    try {
        bp::object module = bp::import("lsst.pex.exceptions");
        boost::shared_ptr<Exception> errPtr(err.clone());
        bp::object errWrapped(errPtr);
        bp::object pyExceptionClass = module.attr("LsstCppException");
        bp::object pyException = pyExceptionClass(errWrapped);
        PyErr_SetObject(pyExceptionClass.ptr(), pyException.ptr());
    } catch (bp::error_already_set & exc) {
        throw exc;
    }
}

void PyException::declare() {
    bp::class_<Tracepoint>("Tracepoint", bp::no_init)
        .def_readonly("_file", &Tracepoint::_file)
        .def_readonly("_line", &Tracepoint::_line)
        .def_readonly("_func", &Tracepoint::_func)
        .def_readonly("_msg", &Tracepoint::_msg)
        ;
    bp::class_<Traceback>("Traceback", bp::no_init)
        .def(bp::vector_indexing_suite<Traceback>())
        ;
    bp::object cppException = bp::class_< Exception, boost::shared_ptr<Exception> >(
        "Exception", bp::no_init
    )
        .def("what", &Exception::what)
        .def("getType", &Exception::getType)
        .def("clone", &Exception::clone, bp::return_value_policy<bp::manage_new_object>())
        .def(str(bp::self))
        .def("getTraceback", &Exception::getTraceback, bp::return_internal_reference<>())
        ;
    FromPython<Exception>::declare();
    bp::register_exception_translator<Exception>(&translate);
}

}}}

BOOST_PYTHON_MODULE(exceptionsLib) {
    bp::scope module;
    bp::object pyLsstException(
        bp::handle<>(
            PyErr_NewException(const_cast<char*>("lsst.pex.exceptions.LsstException"), NULL, NULL)
        )
    );
    bp::object pyLsstCppException(
        bp::handle<>(
            PyErr_NewException(const_cast<char*>("lsst.pex.exceptions.LsstCppException"), 
                               pyLsstException.ptr(), NULL)
        )
    );
    module.attr("LsstException") = pyLsstException;
    module.attr("LsstCppException") = pyLsstCppException;
    lsst::pex::exceptions::PyException::declare();
    DECLARE_LOCAL_SUBCLASS(LogicErrorException, Exception);
    DECLARE_LOCAL_SUBCLASS(DomainErrorException, LogicErrorException);
    DECLARE_LOCAL_SUBCLASS(InvalidParameterException, LogicErrorException);
    DECLARE_LOCAL_SUBCLASS(LengthErrorException, LogicErrorException);
    DECLARE_LOCAL_SUBCLASS(OutOfRangeException, LogicErrorException);
    DECLARE_LOCAL_SUBCLASS(RuntimeErrorException, Exception);
    DECLARE_LOCAL_SUBCLASS(RangeErrorException, RuntimeErrorException);
    DECLARE_LOCAL_SUBCLASS(OverflowErrorException, RuntimeErrorException);
    DECLARE_LOCAL_SUBCLASS(UnderflowErrorException, RuntimeErrorException);
    DECLARE_LOCAL_SUBCLASS(MemoryException, RuntimeErrorException);
    DECLARE_LOCAL_SUBCLASS(IoErrorException, RuntimeErrorException);
    DECLARE_LOCAL_SUBCLASS(TimeoutException, RuntimeErrorException);
    DECLARE_LOCAL_SUBCLASS(NotFoundException, Exception);
}

#undef DECLARE_LOCAL_SUBCLASS
