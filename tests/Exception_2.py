import lsst.pex.exceptions
import failer
import sys

x = failer.Failer()
try:
    x.fail()
except lsst.pex.exceptions.Exception, e:
    print "Caught Exception"
    print e.what()
except failer.MyException, e:
    print "Caught MyException"
    print e.what()
    print issubclass(sys.exc_type, lsst.pex.exceptions.Exception)
    print isinstance(sys.exc_value, failer.MyException)
    print isinstance(sys.exc_value, lsst.pex.exceptions.Exception)
except:
    print issubclass(sys.exc_type, lsst.pex.exceptions.Exception)
    print isinstance(sys.exc_value, failer.MyException)
    print isinstance(sys.exc_value, lsst.pex.exceptions.Exception)
    print sys.exc_info()
