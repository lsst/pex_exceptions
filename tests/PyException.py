# file: PyException.py
import pdb
import sys
import unittest
import lsst.utils.tests as tests
import lsst.pex.exceptions as ex
import exceptionTests
from lsst.daf.data import *

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

class MalformedStackTestCase(unittest.TestCase):
    """ A test case for PyException - malformed stacks"""

    def testMalformed1(self):
        self.assertRaises(AttributeError, ex.LsstFitsError, "Bad ExceptionStack structure", "name")

    def testMalformed2(self):
        self.assertRaises(RuntimeError, ex.LsstFitsError, "Bad ExceptionStack structure",
                          DataProperty("name1", "value1"))

    def testMalformed3(self):
        root = SupportFactory.createPropertyNode("root")
        self.assertRaises(RuntimeError, ex.LsstFitsError, "Bad ExceptionStack structure", root)


#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class WellformedStackTestCase(unittest.TestCase):
    """ A test case for PyException - well-formed stacks"""

    def testMinimalist(self):
        root = SupportFactory.createPropertyNode("root")
        exc1 = SupportFactory.createPropertyNode("exc1")
        root.addProperty(exc1)
        exception = ex.LsstFitsError("Error Message", root)
        rootContents = exception.getStack()
        self.assertEqual("root",rootContents.getName())
        nodes = rootContents.getChildren()
        self.assertEqual(len([1 for i in nodes]),1)
        nodes = rootContents.getChildren()
        for i in nodes:
            prop = i.getChildren()
            self.assertEqual(len([1 for i in prop]),0)


    def testSimple(self):
        root = SupportFactory.createPropertyNode("root")
        exc1 = SupportFactory.createPropertyNode("exc1")
        exc1.addProperty(DataProperty("name1", "value1"))
        root.addProperty(exc1)
        exc2 = SupportFactory.createPropertyNode("exc2")
        exc2.addProperty(DataProperty("name2", 2))
        root.addProperty(exc2)
        exc3 = SupportFactory.createPropertyNode("exc3")
        exc3.addProperty(DataProperty("name3", "value3"))
        root.addProperty(exc3)
        exception = ex.LsstFitsError("Error Message", root)
        rootContents = exception.getStack()
        self.assertEqual("root",rootContents.getName())
        nodes = rootContents.getChildren()
        self.assertEqual(len([1 for i in nodes]),3)
        nodes = rootContents.getChildren()
        for i in nodes:
            prop = i.getChildren()
            self.assertEqual(len([1 for i in prop]),1)


    def testComplex(self):
        root = SupportFactory.createPropertyNode("root")
        exc1 = SupportFactory.createPropertyNode("exc1")
        exc1.addProperty(DataProperty("name1", "value1"))
        root.addProperty(exc1)
        exc2 = SupportFactory.createPropertyNode("exc2")
        exc2.addProperty(DataProperty("name2", 2))
        exc2.addProperty(DataProperty("name2a", "value2a"))
        root.addProperty(exc2)
        exc3 = SupportFactory.createPropertyNode("exc3")
        exc3.addProperty(DataProperty("name3", "value3"))
        root.addProperty(exc3)
        exception = ex.LsstFitsError("Error Message", root)
        rootContents = exception.getStack()
        self.assertEqual("root",rootContents.getName())
        nodes = rootContents.getChildren()
        for i in nodes:
            prop = i.getChildren()
            for j in prop:
                assert not j.isNode() , "DataProperty is a node, not a leaf"


#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class StackManipulationTestCase(unittest.TestCase):
    """ A test case for PyException - stack manipulation"""

    def testGetLast(self):
        root = SupportFactory.createPropertyNode("root")
        exc1 = SupportFactory.createPropertyNode("exc1")
        exc1.addProperty(DataProperty("name1", "value1"))
        root.addProperty(exc1)
        exc2 = SupportFactory.createPropertyNode("exc2")
        exc2.addProperty(DataProperty("name2", 2))
        exc2.addProperty(DataProperty("name2a", "value2a"))
        root.addProperty(exc2)
        exc3 = SupportFactory.createPropertyNode("exc3")
        exc3.addProperty(DataProperty("name3", "value3"))
        exc3.addProperty(DataProperty("name3a", "value3a"))
        exc3.addProperty(DataProperty("name3b", "value3b"))
        exc3.addProperty(DataProperty("name3c", "value3c"))
        root.addProperty(exc3)
        exception = ex.LsstFitsError("Error Message", root)
        rootContents = exception.getLast()
        self.assertEqual("exc3",rootContents.getName())
        nodes = rootContents.getChildren()
        self.assertEqual(len([1 for i in nodes]),4)
        nodes = rootContents.getChildren()
        for i in nodes:
            assert not i.isNode() , "DataProperty is a node, not a leaf"


#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class RaiseFromPythonTestCase(unittest.TestCase):
    """A test case for PyException - throwing exceptions from Python"""

    def testThrow(self):
        root = SupportFactory.createPropertyNode("root")
        exc1 = SupportFactory.createPropertyNode("exc1")
        exc1.addProperty(DataProperty("name1", "value1"))
        root.addProperty(exc1)
        exc2 = SupportFactory.createPropertyNode("exc2")
        exc2.addProperty(DataProperty("name2", 2))
        exc2.addProperty(DataProperty("name2a", "value2a"))
        root.addProperty(exc2)
        exc3 = SupportFactory.createPropertyNode("exc3")
        exc3.addProperty(DataProperty("name3", "value3"))
        exc3.addProperty(DataProperty("name3a", "value3a"))
        exc3.addProperty(DataProperty("name3b", "value3b"))
        exc3.addProperty(DataProperty("name3c", "value3c"))
        root.addProperty(exc3)
        try:
            raise ex.LsstInvalidParameter("urg")
        except ex.LsstInvalidParameter, e:
            print e
        except:
            print sys.exc_info()
            self.fail("caught unexpected exception")
        else:
            self.fail("expected to catch an exception")
        try:
            raise ex.LsstInvalidParameter("arg", root)
        except ex.LsstInvalidParameter, e:
            print e
        except:
            print sys.exc_info()
            self.fail("caught unexpected exception")
        else:
            self.fail("expected to catch an exception")
        try:
            raise ex.LsstInvalidParameter("oof")
        except ex.LsstExceptionStack, e:
            print e
        except:
            print sys.exc_info()
            self.fail("caught unexpected exception")
        else:
            self.fail("expected an exception, none caught")


#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class CppExceptionStackTestCase(unittest.TestCase):
    """ A test case for PyException - C++ exception handling"""
    def setUp(self):
        pass
    def tearDown(self):
        pass

    def testCppExceptionStackUse(self):
        self.assertRaises(ex.LsstDomainError,      exceptionTests.throwDomainError)
        self.assertRaises(ex.LsstInvalidParameter, exceptionTests.throwInvalidParameter)
        self.assertRaises(ex.LsstLengthError,      exceptionTests.throwLengthError)
        self.assertRaises(ex.LsstOutOfRange,       exceptionTests.throwOutOfRange)
        self.assertRaises(ex.LsstRuntime,          exceptionTests.throwRuntime)
        self.assertRaises(ex.LsstRangeError,       exceptionTests.throwRangeError)
        self.assertRaises(ex.LsstOverflow,         exceptionTests.throwOverflow)
        self.assertRaises(ex.LsstUnderflow,        exceptionTests.throwUnderflow)
        self.assertRaises(ex.LsstNotFound,         exceptionTests.throwNotFound)
        self.assertRaises(ex.LsstMemory,           exceptionTests.throwMemory)
        self.assertRaises(ex.LsstFitsError,        exceptionTests.throwFitsError)
        # test inheritance
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwDomainError)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwInvalidParameter)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwLengthError)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwOutOfRange)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwRuntime)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwRangeError)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwOverflow)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwUnderflow)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwNotFound)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwMemory)
        self.assertRaises(ex.LsstExceptionStack,   exceptionTests.throwFitsError)
        # test chained exceptions and make sure getLast() works
        try:
            exceptionTests.throwChain("oh no")
        except ex.LsstExceptionStack, e:
            print e
            last = e.getLast()
            print 'getLast():'
            print last.toString('    ', True)
            assert last.getName() == "oh no"
        except:
            print sys.exc_info()
            self.fail("caught unexpected exception")
        else:
            self.fail("expected an exception, none caught")


#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
def suite():
    """Returns a suite containing all the test cases in this module."""
    tests.init()

    suites = []
    suites += unittest.makeSuite(MalformedStackTestCase)
    suites += unittest.makeSuite(WellformedStackTestCase)
    suites += unittest.makeSuite(StackManipulationTestCase)
    suites += unittest.makeSuite(RaiseFromPythonTestCase)
    suites += unittest.makeSuite(CppExceptionStackTestCase)
    suites += unittest.makeSuite(tests.MemoryTestCase)
    return unittest.TestSuite(suites)

if __name__ == "__main__":
    tests.run(suite())

