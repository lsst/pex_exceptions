#ifndef Exception_1_h
#define Exception_1_h

#include "lsst/pex/exceptions/Exception.h"

namespace pexExcept = lsst::pex::exceptions;

LSST_EXCEPTION_TYPE(ChildException, pexExcept::Exception, lsst.pex.exceptions.Exception)

class DetailedException : public pexExcept::Exception {
public:
    DetailedException(LSST_EARGS_TYPED, int count) :
        Exception(LSST_EARGS_UNTYPED), _count(count) { };
    virtual std::ostream& addToStream(std::ostream& stream) const {
        // Booby-trapped to demonstrate what() exception handling.
        if (_count < 0) throw std::exception();
        // Demonstrates how to add information before and after base class.
        return pexExcept::Exception::addToStream(
            stream << "Begin DetailedException(" << _count <<
            ")" << std::endl) <<
            "End DetailedException" << std::endl;
    };
    int getCount(void) const { return _count; };
private:
    int _count;
};

#endif
