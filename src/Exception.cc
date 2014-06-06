// -*- lsst-c++ -*-
/*
 * LSST Data Management System
 * Copyright 2008-2014 LSST Corporation.
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
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
 * You should have received a copy of the LSST License Statement and
 * the GNU General Public License along with this program.  If not,
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */

#ifndef __GNUC__
#  define __attribute__(x) /*NOTHING*/
#endif

#include <cstring>
#include <ostream>
#include <sstream>
#include <string>

#include "lsst/pex/exceptions/Exception.h"

namespace lsst { namespace pex { namespace exceptions {

Tracepoint::Tracepoint(void) : _file(0), _line(-1), _func(0) {}

Tracepoint::Tracepoint(
    char const* file, int line, char const* func,
    std::string const& message
) :
    _file(file), _line(line), _func(func), _msg(message)
{}

Exception::Exception(
    char const* file, int line, char const* func,
    std::string const& message
) {
    _traceback.push_back(Tracepoint(file, line, func, message));
}

Exception::~Exception(void) throw() {}

void Exception::addMessage(
    char const* file, int line, char const* func, std::string const& message) {
    _traceback.push_back(Tracepoint(file, line, func, message));
}

Traceback const&
    Exception::getTraceback(void) const throw() {
    return _traceback;
}

std::ostream& Exception::addToStream(std::ostream& stream) const {
    if (_traceback.size() > 0) { // Should always be true
        std::string type(getType(), 0, std::strlen(getType()) - 2);
        stream << "0: " << type << " thrown at " <<
            _traceback[0]._file << ":" << _traceback[0]._line << " in " <<
            _traceback[0]._func << std::endl;
        stream << "0: Message: " << _traceback[0]._msg << std::endl;
        for (size_t i = 1; i < _traceback.size(); ++i) {
            stream << i << ": Rethrown at " <<
                _traceback[i]._file << ":" << _traceback[i]._line << " in " <<
                _traceback[i]._func << std::endl;
            stream << i << ": Message: " << _traceback[i]._msg << std::endl;
        }
    }
    return stream;
}

char const* Exception::what(void) const throw() {
    // TODO: is this a concurrency risk?
    static std::string buffer; // static to avoid memory issues
    try {
        std::ostringstream s;
        addToStream(s);
        buffer = s.str(); // copies underlying string
        return buffer.c_str();
    }
    catch (...) {
        return getType();
    }
}

char const* Exception::getType(void) const throw() {
    return "lsst::pex::exceptions::Exception *";
}

Exception* Exception::clone(void) const {
    return new Exception(*this);
}

std::ostream& operator<<(
    std::ostream& stream, Exception const& e) {
    return e.addToStream(stream);
}

}}} // namespace lsst::pex::exceptions
