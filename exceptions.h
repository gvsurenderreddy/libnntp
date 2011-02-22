#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H 1

#include <stdexcept>
#include <string>

namespace nntp
{
    class network_exception : std::runtime_error { public: network_exception(   const std::string& what_arg) : runtime_error(what_arg) {} };
    class server_exception  : std::runtime_error { public: server_exception(    const std::string& what_arg) : runtime_error(what_arg) {} };
    class decode_exception  : std::runtime_error { public: decode_exception(    const std::string& what_arg) : runtime_error(what_arg) {} };
}

#endif /* EXCEPTIONS_H */
