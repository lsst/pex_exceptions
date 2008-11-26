import unittest

import lsst.pex.exceptions
import failer

class ExceptionTestCase(unittest.TestCase):
    """A test case for C++/Python LsstCppExceptions."""
    def setUp(self):
        """Create a wrapped C++ instance that throws an exception."""
        self.x = failer.Failer()

    def tearDown(self):
        del self.x

    def testHierarchy(self):
        """Check that the LsstCppException class hierarchy is correct and that
        the C++ exception is a subclass of lsst.pex.exceptions.Exception."""
        issubclass(lsst.pex.exceptions.LsstException, Exception)
        issubclass(lsst.pex.exceptions.LsstCppException,
                lsst.pex.exceptions.LsstException)
        issubclass(failer.MyException, lsst.pex.exceptions.Exception)

    def testThrow(self):
        """Check that the exception thrown is a Python Exception, an
        LsstException, and an LsstCppException."""
        self.assertRaises(Exception, self.x.fail)
        self.assertRaises(lsst.pex.exceptions.LsstException, self.x.fail)
        self.assertRaises(lsst.pex.exceptions.LsstCppException, self.x.fail)

    def testValue(self):
        """Check that the exception value is a proper SWIGged C++ exception
        with appropriate message."""
        try:
            self.x.fail()
        except Exception, e:
            self.assert_(e is not None)
            self.assertEqual(len(e.args), 1)
            self.assert_(isinstance(e, Exception))
            self.assert_(isinstance(e, lsst.pex.exceptions.LsstException))
            self.assert_(isinstance(e, lsst.pex.exceptions.LsstCppException))
            self.assert_(isinstance(e.args[0], lsst.pex.exceptions.Exception))
            self.assert_(isinstance(e.args[0], failer.MyException))
            self.assertEqual(e.args[0].what(),
                    "0: failer::MyException thrown at tests/Failer.cc:7 " +
                    "in void failer::Failer::fail()\n0: Message: message\n")
            self.assertEqual(str(e),
                    "0: failer::MyException thrown at tests/Failer.cc:7 " +
                    "in void failer::Failer::fail()\n0: Message: message\n")
            self.assertEqual(e.args[0].getType(), "failer::MyException *")

if __name__ == '__main__':
    unittest.main()
