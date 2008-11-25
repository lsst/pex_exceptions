// -*- lsst-c++ -*-

/** \file
 * \brief Implementation of Exception class.
 *
 * \author $Author$
 * \version $Revision$
 * \date $Date$
 *
 * Contact: Kian-Tat Lim (ktl@slac.stanford.edu)
 *
 * \ingroup pex_exceptions
 */

#ifndef __GNUC__
#  define __attribute__(x) /*NOTHING*/
#endif
static char const* SVNid __attribute__((unused)) = "$Id$";

#include "lsst/pex/exceptions/Exception.h"

#include <sstream>

namespace pexExcept = lsst::pex::exceptions;

/** Constructor.
  * \param[in] type Exception's C++ type (automatically passed in by macro).
  * \param[in] trace File, line, function (automatically passed in by macro).
  * \param[in] message Informational string attached to exception.
  */
pexExcept::Exception::Exception(
    char const* type, Tracepoint const& trace, std::string const& message) :
    _type(type) {
    _traceback.push_back(trace);
    _messages.push_back(message);
}

/** Destructor.
  *
  * Not allowed to throw an exception itself.
  */
pexExcept::Exception::~Exception(void) throw() {
}

/** Add a tracepoint and a message to an exception before rethrowing it.
  * \param[in] trace File, line, function (automatically passed in by macro).
  * \param[in] message Additional message to associate with this rethrow.
  */
void pexExcept::Exception::addMessage(Tracepoint const& trace,
                                      std::string const& message) {
    _traceback.push_back(trace);
    _messages.push_back(message);
}

/** Retrieve the list of tracepoints associated with an exception.
  * \return Vector of tracepoints.
  */
pexExcept::Exception::Traceback const&
    pexExcept::Exception::getTraceback(void) const throw() {
    return _traceback;
}

/** Retrieve the list of messages associated with an exception.
  * \return Vector of messages, each associated with a tracepoint.
  */
std::vector<std::string> const&
    pexExcept::Exception::getMessages(void) const throw() {
    return _messages;
}

/** Add a text representation of this exception, including its traceback and
  * messages, to a stream.
  * \param[in] stream Reference to an output stream.
  * \return Reference to the output stream after adding the text.
  */
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

/** Return a character string representing this exception.  Not allowed to
  * throw any exceptions.  Try to use addToStream(); fall back on _type
  * if that fails.
  * \return String representation; does not need to be freed/deleted.
  */
char const* pexExcept::Exception::what(void) const throw() {
    static std::string buffer;
    try {
        buffer.clear();
        std::ostringstream s(buffer);
        addToStream(s);
        return s.str().c_str();
    }
    catch (...) {
        return _type;
    }
}

/** Return the C++ type of the exception.
  * \return String with the C++ type; does not need to be freed/deleted.
  */
char const* pexExcept::Exception::getType(void) const throw() {
    return _type;
}

/** Return the fully-specified C++ type of a pointer to the exception.  This
  * is overridden by derived classes (automatically if the LSST_EXCEPTION_TYPE
  * macro is used.  It is used by the SWIG interface.
  * \return String with the C++ type; does not need to be freed/deleted.
  */
char const* pexExcept::Exception::ctype(void) const throw() {
    return "lsst::pex::exceptions::Exception *";
}

/** Push the text representation of an exception onto a stream.
  * \param[in] stream Reference to an output stream.
  * \param[in] e Exception to output.
  * \return Reference to the output stream after adding the text.
  */
std::ostream& pexExcept::operator<<(
    std::ostream& stream, pexExcept::Exception const& e) {
    return e.addToStream(stream);
}
