from . import exceptionsLib
import lsst.bputils

@lsst.bputils.extend(exceptionsLib.Exception)
class Exception(object):

    def __str__(self):
        return str(self.args[0])

    def getType(self):
        return self.args[0].getType()

    def getTraceback(self):
        return self.args[0].getTraceback()

    def what(self):
        return self.args[0].what()

lsst.bputils.rescope(exceptionsLib, globals(), ignore=("Exception",))
