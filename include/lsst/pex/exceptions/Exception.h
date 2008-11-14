#ifndef LSST_PEX_EXCEPTIONS_EXCEPTION_H
#define LSST_PEX_EXCEPTIONS_EXCEPTION_H

#include <exception>
#include <ostream>
#include <vector>

#include "boost/current_function.hpp"


namespace lsst {
namespace pex {
namespace exceptions {

#define LSST_EXCEPT_HERE \
    lsst::pex::exceptions::Exception::Tracepoint( \
        __FILE__, __LINE__, BOOST_CURRENT_FUNCTION)
#define LSST_EXCEPT(type, ...) type(#type, LSST_EXCEPT_HERE, __VA_ARGS__)
#define LSST_EXCEPT_ADD_HERE(e) e.addMessage(LSST_EXCEPT_HERE)
#define LSST_EXCEPT_ADD_MESSAGE(e, m) e.addMessage(LSST_EXCEPT_HERE, m)

#define LSST_EARGS_TYPED char const* ex_type, lsst::pex::exceptions::Exception::Tracepoint const& ex_trace, std::string const& ex_message
#define LSST_EARGS_UNTYPED ex_type, ex_trace, ex_message

#define LSST_EXCEPTION_TYPE(t, b, p) \
    class t : public b { \
    public: \
        t(LSST_EARGS_TYPED) : b(LSST_EARGS_UNTYPED) { }; \
    };

class Exception : public std::exception {
public:
    struct Tracepoint {
        Tracepoint(char const* file, int line, char const* func) :
            _file(file), _line(line), _func(func) { };
        char const* _file; // Compiled strings only; does not need deletion
        int _line;
        char const* _func; // Compiled strings only; does not need deletion
    };
    typedef std::vector<Tracepoint> Traceback;

    // Constructors
    Exception(LSST_EARGS_TYPED);
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

private:
    char const* _type;
    Traceback _traceback;
    std::vector<std::string> _messages;
};

std::ostream& operator<<(std::ostream& stream, Exception const& e);

}}} // namespace lsst::pex::exceptions

#endif
