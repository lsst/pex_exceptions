#!/usr/bin/env python
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

import sys
import unittest

import lsst.pex.exceptions
import testLib

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
        self.assertRaises(lsst.pex.exceptions.LogicError, testLib.failLogicError1, "message1")
        self.assertRaises(lsst.pex.exceptions.Exception, testLib.failLogicError1, "message1")
        try:
            testLib.failLogicError1("message2")
        except lsst.pex.exceptions.LogicError as err:
            self.assertEqual(err.what(), "message2")
            self.assertEqual(repr(err), "LogicError('message2')")
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

if __name__ == '__main__':
    unittest.main()
