/*
 * This file is part of pex_exceptions.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(LSST_RUNTIME_EXCEPTION)
#define LSST_RUNTIME_EXCEPTION 1

#include "lsst/pex/exceptions/Exception.h"

namespace pexExcept = lsst::pex::exceptions;

namespace lsst {
namespace pex {
namespace exceptions {

LSST_EXCEPTION_TYPE(LogicError, Exception, lsst::pex::exceptions::LogicError)
LSST_EXCEPTION_TYPE(DomainError, LogicError, lsst::pex::exceptions::DomainError)
LSST_EXCEPTION_TYPE(InvalidParameterError, LogicError, lsst::pex::exceptions::InvalidParameterError)
LSST_EXCEPTION_TYPE(LengthError, LogicError, lsst::pex::exceptions::LengthError)
LSST_EXCEPTION_TYPE(OutOfRangeError, LogicError, lsst::pex::exceptions::OutOfRangeError)
LSST_EXCEPTION_TYPE(RuntimeError, Exception, lsst::pex::exceptions::RuntimeError)
LSST_EXCEPTION_TYPE(RangeError, RuntimeError, lsst::pex::exceptions::RangeError)
LSST_EXCEPTION_TYPE(OverflowError, RuntimeError, lsst::pex::exceptions::OverflowError)
LSST_EXCEPTION_TYPE(UnderflowError, RuntimeError, lsst::pex::exceptions::UnderflowError)
LSST_EXCEPTION_TYPE(NotFoundError, Exception, lsst::pex::exceptions::NotFoundError)
LSST_EXCEPTION_TYPE(IoError, RuntimeError, lsst::pex::exceptions::IoError)
LSST_EXCEPTION_TYPE(TypeError, RuntimeError, lsst::pex::exceptions::TypeError)

}  // namespace exceptions
}  // namespace pex
}  // namespace lsst

#endif
