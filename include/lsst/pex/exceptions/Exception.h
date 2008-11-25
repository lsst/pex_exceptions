#ifndef LSST_PEX_EXCEPTIONS_EXCEPTION_H
#define LSST_PEX_EXCEPTIONS_EXCEPTION_H

#include <exception>
#include <ostream>
#include <vector>

#include "boost/current_function.hpp"


namespace lsst {
namespace pex {
namespace exceptions {

/** For internal use; constructs a tracepoint from the file, line, and
  * function.
  */
#define LSST_EXCEPT_HERE \
    lsst::pex::exceptions::Tracepoint( \
        __FILE__, __LINE__, BOOST_CURRENT_FUNCTION)

/** Create an exception with a given type and message and optionally other
  * arguments (dependent on the type).
  * \param[in] type C++ type of the exception to be thrown.
  */
#define LSST_EXCEPT(type, ...) type(#type, LSST_EXCEPT_HERE, __VA_ARGS__)

/** Add the current location and a message to an existing exception before
  * rethrowing it.
  */
#define LSST_EXCEPT_ADD(e, m) e.addMessage(LSST_EXCEPT_HERE, m)

/** The initial arguments required for new exception subclasses.
  */
#define LSST_EARGS_TYPED char const* ex_type, lsst::pex::exceptions::Tracepoint const& ex_trace, std::string const& ex_message

/** The initial arguments to the base class constructor for new subclasses.
  */
#define LSST_EARGS_UNTYPED ex_type, ex_trace, ex_message

/** Macro used to define new types of exceptions without additional data.
  * \param[in] t Type of the exception.
  * \param[in] b Base class of the exception.
  * \param[in] c C++ class of the exception (fully specified).
  */
#define LSST_EXCEPTION_TYPE(t, b, c) \
    class t : public b { \
    public: \
        t(LSST_EARGS_TYPED) : b(LSST_EARGS_UNTYPED) { }; \
        virtual char const* ctype(void) const throw() { return #c " *"; }; \
    };

struct Tracepoint {
    Tracepoint(char const* file, int line, char const* func) :
        _file(file), _line(line), _func(func) { };
    char const* _file; // Compiled strings only; does not need deletion
    int _line;
    char const* _func; // Compiled strings only; does not need deletion
};

class Exception : public std::exception {
public:
    typedef std::vector<Tracepoint> Traceback;

    // Constructors
    // Should use LSST_EARGS_TYPED, but that confuses doxygen.
    Exception(char const* type,
              lsst::pex::exceptions::Tracepoint const& trace,
              std::string const& message);
    virtual ~Exception(void) throw();

    // Modifiers
    void addMessage(Tracepoint const& trace,
                    std::string const& message = std::string());

    // Accessors
    Traceback const& getTraceback(void) const throw();
    std::vector<std::string> const& getMessages(void) const throw();
    virtual std::ostream& addToStream(std::ostream& stream) const;
    virtual char const* what(void) const throw();
    char const* getType(void) const throw();
    virtual char const* ctype(void) const throw();

private:
    char const* _type;
    Traceback _traceback;
    std::vector<std::string> _messages;
};

std::ostream& operator<<(std::ostream& stream, Exception const& e);

}}} // namespace lsst::pex::exceptions

#endif
