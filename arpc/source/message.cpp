
#include "message.hpp"

namespace arpc {

Message::Message(uint16_t type, uint16_t version)
    : m_header{type, version, 0, 0} {}

uint16_t Message::GetType() {return m_header.type;}

uint16_t Message::GetVersion() {return m_header.version;}

uint64_t Message::GetId() {return m_header.id;}

const unsigned char* Message::PackHeader() {
    return (const unsigned char*)&m_header;
}

size_t Message::PackHeaderSize() {
    return sizeof(m_header);
}

void Message::UnpackHeader(const unsigned char* buffer, size_t size) {}

}  // namespace arpc