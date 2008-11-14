#include "lsst/pex/exceptions/Exception.h"

#include <iomanip>
#include <sstream>

namespace pexExcept = lsst::pex::exceptions;

pexExcept::Exception::Exception(
    char const* type, Tracepoint const& trace, std::string const& message) :
    _type(type) {
    _traceback.push_back(trace);
    _messages.push_back(message);
}

pexExcept::Exception::~Exception(void) throw() {
}

void pexExcept::Exception::addMessage(Tracepoint const& trace,
                                      std::string const& message) {
    _traceback.push_back(trace);
    _messages.push_back(message);
}

pexExcept::Exception::Traceback const&
    pexExcept::Exception::getTraceback(void) const throw() {
    return _traceback;
}

std::vector<std::string> const&
    pexExcept::Exception::getMessages(void) const throw() {
    return _messages;
}

std::ostream& pexExcept::Exception::addToStream(std::ostream& stream) const {
    if (_traceback.size() > 0) { // Should always be true
        stream << "0: " << _type << " thrown at " <<
            _traceback[0]._file << ":" << _traceback[0]._line << " in " <<
            _traceback[0]._func << std::endl;
        if (_messages[0].size() > 0) {
            stream << "0: Message: " << _messages[0] << std::endl;
        }
        for (size_t i = 1; i < _traceback.size(); ++i) {
            stream << i << ": Rethrown at " <<
                _traceback[i]._file << ":" << _traceback[i]._line << " in " <<
                _traceback[i]._func << std::endl;
            if (_messages[i].size() > 0) {
                stream << i << ": Message: " << _messages[i] << std::endl;
            }
        }
    }
    return stream;
}

char const* pexExcept::Exception::what(void) const throw() {
    try {
        std::ostringstream s;
        addToStream(s);
        return s.str().c_str();
    }
    catch (...) {
        return std::exception::what();
    }
}

char const* pexExcept::Exception::getType(void) const throw() {
    return _type;
}

std::ostream& pexExcept::operator<<(
    std::ostream& stream, pexExcept::Exception const& e) {
    return e.addToStream(stream);
}
