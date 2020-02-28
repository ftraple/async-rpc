#ifndef ARPC_MESSAGE_HPP_
#define ARPC_MESSAGE_HPP_

#include <string>
#include <cstdint>
#include <vector>

namespace arpc {

typedef struct {
    size_t size;
    unsigned char* data;
} MessagePack;

class Message {

    public:

    struct Header {
        uint16_t type;
        uint16_t version;
        uint64_t id;
        uint32_t body_size;
    };

    Message(uint16_t type, uint16_t version);

    const MessagePack& Pack();

    void Unpack(const MessagePack& data_pack) const;

    protected:

    Header m_header;
};

}  // namespace arpc

#endif  // ARPC_MESSAGE_HPP_