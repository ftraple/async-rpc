
#include "message-handler.hpp"

namespace arpc {

MessageHandler::MessageHandler(IConnection& connection)
    : m_connection(connection) {}

void MessageHandler::RegisterMessage(int type, int version,
                                     std::function<void(uint16_t, uint16_t)> create_message,
                                     std::function<void(const Message&)> caller) {
    m_caller_list[std::make_tuple(type, version)] = caller;
}

void MessageHandler::ReceiveMessage(std::chrono::milliseconds timeout) {
    Message::Header message_header;
    m_connection.Receive((unsigned char*)&message_header, sizeof(message_header), timeout);
    Message message = CreateMessage(message_header.type, message_header.version);
    if (message_header.body_size > 0) {
        unsigned char* message_body = new unsigned char[message_header.body_size];
        // TODO Ckeck
        m_connection.Receive(message_body, message_header.body_size, timeout);
        //message.Unpack(message_body);
        delete[] message_body;
    }
    // Execute handler function
}

void MessageHandler::SendMessage(const Message& message) {
    //MessagePack message_pack = message.Pack();
    //m_connection.Send(message_pack.data, message_pack.size);
}

}  // namespace arpc