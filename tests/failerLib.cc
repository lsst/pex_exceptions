#include "lsst/pex/exceptions/python.h"
#include "Failer.h"

namespace bp = boost::python;
namespace pexExcept = lsst::pex::exceptions;

BOOST_PYTHON_MODULE(failer) {
    
    bp::import("lsst.pex.exceptions");

    pexExcept::PyException::subclass<failer::MyException,pexExcept::Exception>("MyException");
    
    bp::class_<failer::Failer>("Failer")
        .def("fail", &failer::Failer::fail)
        .def("fail2", &failer::Failer::fail2)
        ;
};
