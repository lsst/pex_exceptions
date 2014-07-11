from __future__ import print_function

import sys
import traceback

from . import exceptionsLib

class Exception(StandardError):
    """The base class for Python-wrapped LSST C++ exceptions.
    """

    WrappedClass = exceptionsLib.Exception

    def __init__(self, arg):
        if isinstance(arg, exceptionsLib.Exception):
            cpp = arg
            message = " ".join(tp._msg for tp in cpp.getTraceback())
        else:
            message = arg
            file, line, func, _ = traceback.extract_stack()[-2]
            cpp = self.WrappedClass(file, line, func, message)
        StandardError.__init__(self, message)
        self.cpp = cpp

    def addMessage(self, message):
        """Add a message to the Exception, using the C++ Exception's Traceback vector.
        This message will not appear in str(self) or repr(self), but it will appear
        in Python tracebacks
        """
        file, line, func, _ = traceback.extract_stack()[-2]
        self.cpp.addMessage(file, line, func, message)
        self.args = (" ".join(tp._msg for tp in self.cpp.getTraceback()),)

    def formatCppTraceback(self):
        return '\n'.join('  File "%s", line %d, in %s: %s' % (tp._file, tp._line, tp._func, tp._msg)
                         for tp in self.cpp.getTraceback())

def excepthook(type, value, tb):
    traceback.print_exception(type, value, tb, file=sys.stderr)
    if issubclass(type, Exception):
        print(value.formatCppTraceback(), file=sys.stderr)
    print("%s: %s" % (type.__name__, value), file=sys.stderr)
sys.excepthook = excepthook
