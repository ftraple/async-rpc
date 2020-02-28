
#include "message.hpp"

namespace arpc {

Message::Message(uint16_t type, uint16_t version)
    : m_header{type, version, 0} {}

uint16_t Message::GetType() {return m_header.type;}

uint16_t Message::GetVersion() {return m_header.version;}

uint64_t Message::GetId() {return m_header.id;}

const unsigned char* Message::Pack() {
    return (const unsigned char*)&m_header;
}

size_t Message::PackSize() {
    return sizeof(m_header);
}

void Message::Unpack(const unsigned char* pack, size_t size) {}

}  // namespace arpc