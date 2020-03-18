
#include "message.hpp"

namespace arpc {

Message::Message(uint16_t type, uint16_t version)
    : m_header{std::make_unique<Header>(type, version, 0, 0)} {}

uint16_t Message::GetType() { return m_header->type; }

uint16_t Message::GetVersion() { return m_header->version; }

uint64_t Message::GetId() { return m_header->id; }

void Message::SetBodySize(uint32_t body_size) {m_header->body_size = body_size;}

const char* Message::PackHeader() {
    return (const char*)m_header.get();
}

size_t Message::PackHeaderSize() {
    return sizeof(Header);
}

}  // namespace arpc