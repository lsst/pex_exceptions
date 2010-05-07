// -*- lsst-c++ -*-
#ifndef LSST_PEX_EXCEPTIONS_PYTHON_H
#define LSST_PEX_EXCEPTIONS_PYTHON_H

#include <boost/python.hpp>
#include "lsst/pex/exceptions.h"

#include <iostream>

namespace lsst { namespace pex { namespace exceptions {

struct PyException {

    template <typename Class, typename Base>
    static void subclass(char const * name) {
        boost::python::object cppException 
            = boost::python::class_< Class, boost::shared_ptr<Class>, boost::python::bases<Base> >(
                name, boost::python::no_init
            );
        FromPython<Class>::declare();
    }

    static void declare();

private:

    static void translate(Exception const & err);

    template <typename Class>
    struct FromPython {
        
        static void declare() {
            boost::python::converter::registry::insert(
                &extract,
                boost::python::type_id<Class>(),
                &boost::python::converter::registered_pytype<Class>::get_pytype
            );
        }

    private:
        
        static void* extract(PyObject * pyObj) {
            boost::python::object cpp = boost::python::object(
                boost::python::handle<>(PyObject_GetAttrString(pyObj, "_cpp"))
            );
            return boost::python::extract<Class*>(cpp)();
        }

    };

};

}}}

#endif // !LSST_PEX_EXCEPTIONS_PYTHON_H
