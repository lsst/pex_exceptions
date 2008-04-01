# -*- python -*-
import lsst.daf.base

def _isValidStack(stack):
    """Returns True if the structure of stack (a DataProperty)
       meets ExceptionStack requirements, False otherwise"""
    if not stack.isNode():
        return False
    children    = stack.getChildren()
    numChildren = 0
    # each child must be a node rather than a leaf
    for i in children:
        numChildren += 1
        if not i.isNode():
            return False
    return numChildren != 0


class LsstExceptionStack(Exception):
    """Base class for Python analogues to C++ LSST exceptions"""

    def __init__(self, what, stack=None):
        """
        Initialize the exception stack.
        
        Input
        what:   message to deliver if this exception stack is used as an exception handler.
        stack:  optional. If provided, used as exception stack. If not provided, a new stack will be created.

        Raise
        RuntimeError if provided stack was not a valid stack.
        """
        if stack is None:
            # Create an exception stack containing a single exception
            name  = self.__class__.__name__
            stack = lsst.daf.data.DataProperty.createPropertyNode(name)
            data  = lsst.daf.data.DataProperty.createPropertyNode("Node" + name)
            stack.addProperty(data)
            self.stack = stack
        elif not _isValidStack(stack):
            raise RuntimeError, 'Malformed ExceptionStack passed to Python'
        else:
            self.stack = stack
        self.what = what

    def __str__(self):
        """ Return string representation of the Exception Stack.  """
        if self.stack is None:
            return self.what
        else:
            return ''.join([self.what, ':\n', self.stack.toString('    ', True)])

    def getStack(self):
        """ Return the Exception Stack.  """
        return self.stack

    def getLast(self):
        """
        Return the last instance of Exception Data installed on the Exception Stack.

        Raise
        RuntimeError if the ExceptionStack contains no ExceptionData.
        """
        if self.stack is not None:
            children = self.stack.getChildren()
            last     = None
            for i in children:
                last = i
            return last
        raise RuntimeError, 'ExceptionStack contains no ExceptionData'


# One Python class per LSST C++ exception class

class LsstDomainError(LsstExceptionStack):
    """ Python interface to C++ LSST DomainError exception handler.  """
    pass

class LsstInvalidParameter(LsstExceptionStack):
    """ Python interface to C++ LSST InvalidParameter exception handler.  """
    pass

class LsstLengthError(LsstExceptionStack):
    """ Python interface to C++ LSST LengthError exception handler.  """
    pass

class LsstOutOfRange(LsstExceptionStack):
    """ Python interface to C++ LSST OutOfRange exception handler.  """
    pass

class LsstRuntime(LsstExceptionStack):
    """ Python interface to C++ LSST Runtime exception handler.  """
    pass

class LsstRangeError(LsstExceptionStack):
    """ Python interface to C++ LSST RangeError exception handler.  """
    pass

class LsstOverflow(LsstExceptionStack):
    """ Python interface to C++ LSST Overflow exception handler.  """
    pass

class LsstUnderflow(LsstExceptionStack):
    """ Python interface to C++ LSST Underflow exception handler.  """
    pass

class LsstNotFound(LsstExceptionStack):
    """ Python interface to C++ LSST NotFound exception handler.  """
    pass

class LsstMemory(LsstExceptionStack):
    """ Python interface to C++ LSST Memory exception handler.  """
    pass

class LsstFitsError(LsstExceptionStack):
    """ Python interface to C++ LSST FitsError exception handler.  """
    pass

