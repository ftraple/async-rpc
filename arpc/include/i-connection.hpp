#ifndef ARPC_I_CONNECTION_HPP_
#define ARPC_I_CONNECTION_HPP_

#include <chrono>
#include <cstdint>
#include <string>

namespace arpc {

class IConnection {
   public:
    virtual bool Connect(const std::string& host, uint16_t port) = 0;

    virtual void Disconnect() = 0;

    virtual bool IsConnected() = 0;

    virtual bool Send(const unsigned char* data, size_t size) = 0;

    virtual bool Receive(unsigned char* data, size_t max_size, std::chrono::milliseconds) = 0;
};

}  // namespace arpc

#endif  // ARPC_I_CONNECTION_HPP_