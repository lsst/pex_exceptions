from exceptionsLib import *

def _Exception__init__(self, cpp):
    self._cpp = cpp
    self.args = (cpp,)
exceptionsLib.Exception.__init__ = _Exception__init__
del _Exception__init__

