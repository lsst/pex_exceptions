from __future__ import absolute_import
import warnings
import builtins

from future.utils import with_metaclass
from . import exceptions


registry = {}


def register(cls):
    """A Python decorator that adds a Python exception wrapper to the registry that maps C++ Exceptions
    to their Python wrapper classes.
    """
    registry[cls.WrappedClass] = cls
    return cls


class ExceptionMeta(type):
    """A metaclass for custom exception wrappers, which adds lookup of class attributes
    by delegating to the Swig-generated wrapper.
    """

    def __getattr__(self, name):
        return getattr(self.WrappedClass, name)


@register
class Exception(with_metaclass(ExceptionMeta, builtins.Exception)):
    """The base class for Python-wrapped LSST C++ exceptions.
    """

    # wrappers.py is an implementation detail, not a public namespace, so we pretend this is defined
    # in the package for pretty-printing purposes
    __module__ = "lsst.pex.exceptions"

    WrappedClass = exceptions.Exception

    def __init__(self, arg, *args, **kwds):
        if isinstance(arg, exceptions.Exception):
            cpp = arg
            message = cpp.what()
        else:
            message = arg
            cpp = self.WrappedClass(message, *args, **kwds)
        super(Exception, self).__init__(message)
        self.cpp = cpp

    def __getattr__(self, name):
        return getattr(self.cpp, name)

    def __repr__(self):
        return "%s('%s')" % (type(self).__name__, self.cpp.what())

    def __str__(self):
        return self.cpp.asString()


@register
class LogicError(Exception):
    WrappedClass = exceptions.LogicError


@register
class DomainError(LogicError):
    WrappedClass = exceptions.DomainError


@register
class InvalidParameterError(LogicError):
    WrappedClass = exceptions.InvalidParameterError


@register
class LengthError(LogicError):
    WrappedClass = exceptions.LengthError


@register
class OutOfRangeError(LogicError):
    WrappedClass = exceptions.OutOfRangeError


@register
class RuntimeError(Exception, builtins.RuntimeError):
    WrappedClass = exceptions.RuntimeError


@register
class RangeError(RuntimeError):
    WrappedClass = exceptions.RangeError


@register
class OverflowError(RuntimeError, builtins.OverflowError):
    WrappedClass = exceptions.OverflowError


@register
class UnderflowError(RuntimeError, builtins.ArithmeticError):
    WrappedClass = exceptions.UnderflowError


@register
class NotFoundError(Exception, builtins.LookupError):
    WrappedClass = exceptions.NotFoundError


@register
class IoError(RuntimeError, builtins.IOError):
    WrappedClass = exceptions.IoError


@register
class TypeError(RuntimeError, builtins.TypeError):
    WrappedClass = exceptions.TypeError


def translate(cpp):
    """Translate a C++ Exception instance to Python and return it."""
    PyType = registry.get(type(cpp), None)
    if PyType is None:
        warnings.warn("Could not find appropriate Python type for C++ Exception")
        PyType = Exception
    return PyType(cpp)


def declare(module, exception_name, base, wrapped_class):
    """Declare a new exception."""
    setattr(module, exception_name, register(ExceptionMeta(exception_name, (base, ),
                                                           dict(WrappedClass=wrapped_class))))
