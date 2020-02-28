
#include "message.hpp"

namespace arpc {

Message::Message(uint16_t type, uint16_t version)
    : m_header{type, version} {}

const MessagePack& Message::Pack() {}

void Message::Unpack(const MessagePack& data_pack) const {}

}  // namespace arpc