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
            print e.__class__
            print dir(e)
            print len(e.args)
            print e
            
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
def run():
    unittest.main()

if __name__ == "__main__":
    run()
