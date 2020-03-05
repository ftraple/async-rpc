#ifndef ARPC_MESSAGE_HPP_
#define ARPC_MESSAGE_HPP_

#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <vector>
#include "pack.hpp"

namespace arpc {

class Message {
   public:
    struct Header {
        uint16_t type;
        uint16_t version;
        uint64_t id;
        uint32_t body_size;
    };

    Message(uint16_t type, uint16_t version);

    ~Message() {};

    uint16_t GetType();

    uint16_t GetVersion();

    uint64_t GetId();

    const unsigned char* PackHeader();

    size_t PackHeaderSize();

    size_t PackBodySize() { return 0; }

    virtual void PackBody(const unsigned char* buffer) {}

    virtual void UnpackBody(const unsigned char* buffer) {}

   protected:
    Header m_header;
};

}  // namespace arpc

#endif  // ARPC_MESSAGE_HPP_