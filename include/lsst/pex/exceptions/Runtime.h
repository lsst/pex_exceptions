// -*- lsst-c++ -*-
#if !defined(LSST_RUNTIME_EXCEPTION)      //! multiple inclusion guard macro
#define LSST_RUNTIME_EXCEPTION 1

#include "lsst/pex/exceptions/Exception.h"

namespace pexExcept = lsst::pex::exceptions;

namespace lsst {
namespace pex {
namespace exceptions {

LSST_EXCEPTION_TYPE(LogicErrorException, Exception, lsst::pex::exceptions::LogicErrorException)
LSST_EXCEPTION_TYPE(DomainErrorException, LogicErrorException, lsst::pex::exceptions::DomainErrorException)
LSST_EXCEPTION_TYPE(InvalidParameterException, LogicErrorException, lsst::pex::exceptions::InvalidParameterException)
LSST_EXCEPTION_TYPE(LengthErrorException, LogicErrorException, lsst::pex::exceptions::LengthErrorException)
LSST_EXCEPTION_TYPE(OutOfRangeException, LogicErrorException, lsst::pex::exceptions::OutOfRangeException)
LSST_EXCEPTION_TYPE(RuntimeErrorException, Exception, lsst::pex::exceptions::RuntimeErrorException)
LSST_EXCEPTION_TYPE(RangeErrorException, RuntimeErrorException, lsst::pex::exceptions::RangeErrorException)
LSST_EXCEPTION_TYPE(OverflowErrorException, RuntimeErrorException, lsst::pex::exceptions::OverflowErrorException)
LSST_EXCEPTION_TYPE(UnderflowErrorException, RuntimeErrorException, lsst::pex::exceptions::UnderflowErrorException)
LSST_EXCEPTION_TYPE(NotFoundException, Exception, lsst::pex::exceptions::NotFoundException)
LSST_EXCEPTION_TYPE(MemoryException, Exception, lsst::pex::exceptions::MemoryException)
LSST_EXCEPTION_TYPE(IoErrorException, Exception, lsst::pex::exceptions::IoErrorException)

}
}
}

#endif
