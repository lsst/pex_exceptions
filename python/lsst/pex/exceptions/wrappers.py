import warnings
import __builtin__

from . import _exceptions as exceptionsLib

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
class Exception(StandardError):
    """The base class for Python-wrapped LSST C++ exceptions.
    """

    __metaclass__ = ExceptionMeta

    # wrappers.py is an implementation detail, not a public namespace, so we pretend this is defined
    # in the package for pretty-printing purposes
    __module__ = "lsst.pex.exceptions"

    WrappedClass = exceptionsLib.Exception

    def __init__(self, arg, *args, **kwds):
        if isinstance(arg, exceptionsLib.Exception):
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
    WrappedClass = exceptionsLib.LogicError

@register
class DomainError(LogicError):
    WrappedClass = exceptionsLib.DomainError

@register
class InvalidParameterError(LogicError):
    WrappedClass = exceptionsLib.InvalidParameterError

@register
class LengthError(LogicError):
    WrappedClass = exceptionsLib.LengthError

@register
class OutOfRangeError(LogicError):
    WrappedClass = exceptionsLib.OutOfRangeError

@register
class RuntimeError(Exception, __builtin__.RuntimeError):
    WrappedClass = exceptionsLib.RuntimeError

@register
class RangeError(RuntimeError):
    WrappedClass = exceptionsLib.RangeError

@register
class OverflowError(RuntimeError, __builtin__.OverflowError):
    WrappedClass = exceptionsLib.OverflowError

@register
class UnderflowError(RuntimeError, __builtin__.ArithmeticError):
    WrappedClass = exceptionsLib.UnderflowError

@register
class NotFoundError(Exception, __builtin__.LookupError):
    WrappedClass = exceptionsLib.NotFoundError

@register
class MemoryError(RuntimeError, __builtin__.MemoryError):
    WrappedClass = exceptionsLib.MemoryError

@register
class IoError(RuntimeError, __builtin__.IOError):
    WrappedClass = exceptionsLib.IoError

@register
class TypeError(RuntimeError, __builtin__.TypeError):
    WrappedClass = exceptionsLib.TypeError

@register
class TimeoutError(RuntimeError):
    WrappedClass = exceptionsLib.TimeoutError

def translate(cpp):
    """Translate a C++ Exception instance to Python and return it."""
    PyType = registry.get(type(cpp), None)
    if PyType is None:
        warnings.warn("Could not find appropriate Python type for C++ Exception")
        PyType = Exception
    return PyType(cpp)

def declare(module, exception_name, base, wrapped_class):
    """Declare a new exception."""
    setattr(module, exception_name, register(ExceptionMeta(exception_name, (base, ), dict(WrappedClass=wrapped_class))))

