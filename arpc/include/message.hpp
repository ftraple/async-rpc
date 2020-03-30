#ifndef ARPC_MESSAGE_HPP_
#define ARPC_MESSAGE_HPP_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "pack.hpp"

namespace arpc {

class Message {
   public:
    struct Header {
        uint16_t type;
        uint16_t version;
        uint32_t body_size;
        Header(uint16_t a_type, uint16_t a_version, uint32_t a_body_size)
            : type{a_type}, version{a_version}, body_size{a_body_size} {}
    }__attribute__((packed));

    Message(uint16_t type, uint16_t version);

    virtual ~Message() {};

    uint16_t GetType();

    uint16_t GetVersion();

    void SetBodySize(uint32_t body_size);

    const char* PackHeader();

    size_t PackHeaderSize();

    size_t PackBodySize() { return 0; }

    virtual void PackBody(char* buffer) {}

    virtual void UnpackBody(const char* buffer) {}

   protected:
    std::unique_ptr<Header> m_header;
};

}  // namespace arpc

#endif  // ARPC_MESSAGE_HPP_