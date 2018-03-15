#
# LSST Data Management System
# Copyright 2008-2014 LSST Corporation.
#
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the LSST License Statement and
# the GNU General Public License along with this program.  If not,
# see <http://www.lsstcorp.org/LegalNotices/>.
#

from builtins import str

import unittest

import lsst.pex.exceptions
import _testLib as testLib


class ExceptionTestCase(unittest.TestCase):
    """A test case for C++/Python LsstCppExceptions."""

    def testBaseTranslation(self):
        self.assertRaises(lsst.pex.exceptions.Exception, testLib.failException1, "message1")
        try:
            testLib.failException1("message2")
        except lsst.pex.exceptions.Exception as err:
            self.assertEqual(err.what(), "message2")
            self.assertEqual(repr(err), "Exception('message2')")
        else:
            self.fail("Expected Exception not raised")

    def testDerivedTranslation(self):
        self.assertRaises(lsst.pex.exceptions.RuntimeError, testLib.failRuntimeError1, "message1")
        self.assertRaises(lsst.pex.exceptions.Exception, testLib.failRuntimeError1, "message1")
        try:
            testLib.failRuntimeError1("message2")
        except lsst.pex.exceptions.RuntimeError as err:
            self.assertEqual(err.what(), "message2")
            self.assertEqual(repr(err), "RuntimeError('message2')")
        else:
            self.fail("Expected Exception not raised")

    def testAddMessage(self):
        try:
            testLib.failLogicError2("message1", "message2")
        except lsst.pex.exceptions.LogicError as err:
            self.assertEqual(err.what(), "message1 {0}; message2 {1}")
            self.assertEqual(repr(err), "LogicError('message1 {0}; message2 {1}')")
        else:
            self.fail("Expected Exception not raised")

    def testPythonRaise(self):
        try:
            raise lsst.pex.exceptions.LogicError("message1")
        except lsst.pex.exceptions.Exception as err:
            self.assertIsInstance(err, lsst.pex.exceptions.LogicError)
            self.assertEqual(err.what(), "message1")
            self.assertEqual(repr(err), "LogicError('message1')")
            self.assertEqual(str(err), "message1")

    def testCustom(self):
        self.assertRaises(lsst.pex.exceptions.Exception, testLib.failTestError1, "message1")
        self.assertRaises(lsst.pex.exceptions.RuntimeError, testLib.failTestError1, "message1")
        self.assertRaises(RuntimeError, testLib.failTestError1, "message1")
        self.assertRaises(testLib.TestError, testLib.failTestError1, "message1")
        try:
            testLib.failTestError1("message2")
        except lsst.pex.exceptions.Exception as err:
            self.assertEqual(err.what(), "message2")
            self.assertEqual(repr(err), "TestError('message2')")
        else:
            self.fail("Expected Exception not raised")

    def checkHierarchy(self, method, classes):
        for cls in classes:
            self.assertRaises(cls, method, "message")

    def testHierarchy(self):
        self.checkHierarchy(testLib.failLogicError1,
                            [lsst.pex.exceptions.LogicError,
                             lsst.pex.exceptions.Exception])
        self.checkHierarchy(testLib.failInvalidParameterError1,
                            [lsst.pex.exceptions.InvalidParameterError,
                             lsst.pex.exceptions.LogicError,
                             lsst.pex.exceptions.Exception])
        self.checkHierarchy(testLib.failLengthError1,
                            [lsst.pex.exceptions.LengthError,
                             lsst.pex.exceptions.LogicError,
                             lsst.pex.exceptions.Exception])
        self.checkHierarchy(testLib.failOutOfRangeError1,
                            [lsst.pex.exceptions.OutOfRangeError,
                             lsst.pex.exceptions.LogicError,
                             lsst.pex.exceptions.Exception])
        self.checkHierarchy(testLib.failRuntimeError1,
                            [lsst.pex.exceptions.RuntimeError,
                             lsst.pex.exceptions.Exception])
        self.checkHierarchy(testLib.failOverflowError1,
                            [lsst.pex.exceptions.OverflowError,
                             lsst.pex.exceptions.RuntimeError,
                             lsst.pex.exceptions.Exception,
                             OverflowError])
        self.checkHierarchy(testLib.failUnderflowError1,
                            [lsst.pex.exceptions.UnderflowError,
                             lsst.pex.exceptions.RuntimeError,
                             lsst.pex.exceptions.Exception,
                             ArithmeticError])
        self.checkHierarchy(testLib.failNotFoundError1,
                            [lsst.pex.exceptions.NotFoundError,
                             lsst.pex.exceptions.Exception,
                             LookupError])
        self.checkHierarchy(testLib.failIoError1,
                            [lsst.pex.exceptions.IoError,
                             lsst.pex.exceptions.RuntimeError,
                             lsst.pex.exceptions.Exception,
                             IOError])
        self.checkHierarchy(testLib.failTypeError1,
                            [lsst.pex.exceptions.TypeError,
                             lsst.pex.exceptions.RuntimeError,
                             lsst.pex.exceptions.Exception,
                             TypeError])


if __name__ == '__main__':
    unittest.main()
