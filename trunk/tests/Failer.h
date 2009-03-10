#ifndef FAILER_H
#define FAILER_H

#include "lsst/pex/exceptions.h"

namespace pexExcept = lsst::pex::exceptions;

namespace failer {

LSST_EXCEPTION_TYPE(MyException, pexExcept::Exception, failer::MyException)

class Failer {
public:
    Failer(void);
    void fail(void);
    void fail2(void) {
        throw LSST_EXCEPT(pexExcept::DomainErrorException, "message 2");
    }
};

}

#endif
