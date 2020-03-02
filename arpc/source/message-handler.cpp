
#include "message-handler.hpp"

namespace arpc {

MessageHandler::MessageHandler(IConnection& connection)
    : m_connection(connection) {}

void MessageHandler::RegisterMessage(int type, int version,
                                     CreateMessageFunction create_message_function,
                                     CallerFunction caller) {
    m_create_message_list[std::make_tuple(type, version)] = create_message_function;
    m_caller_list[std::make_tuple(type, version)] = caller;
}

std::optional<Message*> MessageHandler::CreateMessage(uint16_t type, uint16_t version) {
    auto create_message_function = m_create_message_list[std::make_tuple(type, version)];
    if (!create_message_function) {
        return std::nullopt;
    }
    return create_message_function();
}

CallerFunction MessageHandler::FindCaller(uint16_t type, uint16_t version) {
    auto caller = m_caller_list[std::make_tuple(type, version)];
    if (!caller) {
        return nullptr;
    }
    return caller;
}


void MessageHandler::ReceiveMessage(std::chrono::milliseconds timeout) {
    Message::Header message_header;
    m_connection.Receive((unsigned char*)&message_header, sizeof(message_header), timeout);
    auto message = CreateMessage(message_header.type, message_header.version);
    if (!message) {
        return;
    }
    // if (message_header.body_size > 0) {
    //     unsigned char* message_body = new unsigned char[message_header.body_size];
    //     m_connection.Receive(message_body, message_header.body_size, timeout);
    //     message->Unpack(message_body, message_header.body_size);
    //     delete[] message_body;
    // }
    auto caller = FindCaller(message_header.type, message_header.version);
    caller(*message);
    delete *message;
}

void MessageHandler::SendMessage(Message& message) {
    m_connection.Send(message.Pack(), message.PackSize());
}

}  // namespace arpc