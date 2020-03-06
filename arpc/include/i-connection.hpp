#ifndef ARPC_I_CONNECTION_HPP_
#define ARPC_I_CONNECTION_HPP_

#include <chrono>
#include <cstdint>
#include <string>

namespace arpc {

class IConnection {
   public:
    virtual bool Send(const char* data, size_t size, bool more) = 0;

    virtual bool Receive(char* data, size_t size) = 0;
};

}  // namespace arpc

#endif  // ARPC_I_CONNECTION_HPP_