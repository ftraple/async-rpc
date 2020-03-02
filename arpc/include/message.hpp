#ifndef ARPC_MESSAGE_HPP_
#define ARPC_MESSAGE_HPP_

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

    uint16_t GetType();

    uint16_t GetVersion();

    uint64_t GetId();

    const unsigned char* Pack();

    size_t PackSize();

    void Unpack(const unsigned char* data, size_t size);

    protected:

    Header m_header;
};

}  // namespace arpc

#endif  // ARPC_MESSAGE_HPP_