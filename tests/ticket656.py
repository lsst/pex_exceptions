#!/usr/bin/env python
"""
Tests for ticket656

Run with:
   python ticket656.py
or
   python
   >>> import ticket656; ticket656.run()
"""

import unittest
import lsst.utils.tests as tests
import failer

class Ticket656TestCase(unittest.TestCase):
    """A test case for Measure"""
    def setUp(self):
        """Create a wrapped C++ instance that throws an exception."""
        self.x = failer.Failer()

    def tearDown(self):
        del self.x

    def testFail2(self):
        try:
            self.x.fail2()
        except Exception, e:
            print e
            
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

def suite():
    """Returns a suite containing all the test cases in this module."""
    tests.init()

    suites = []
    suites += unittest.makeSuite(Ticket656TestCase)
    return unittest.TestSuite(suites)

def run(exit=False):
    """Run the tests"""
    tests.run(suite(), exit)

if __name__ == "__main__":
    run(True)
