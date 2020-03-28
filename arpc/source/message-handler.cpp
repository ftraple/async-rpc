
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

Message* MessageHandler::CreateMessage(uint16_t type, uint16_t version) {
    auto create_message_function = m_create_message_list[std::make_tuple(type, version)];
    if (!create_message_function) {
        return nullptr;
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

bool MessageHandler::ReceiveMessage() {
    Message::Header message_header{0, 0, 0, 0};
    if (!m_connection.Receive((char*)&message_header, sizeof(message_header))) {
        return false;
    }
    bool ret{true};
    auto message = CreateMessage(message_header.type, message_header.version);
    if (!message) {
        std::cout << "ARPC: Fail to create the message type [" << message_header.type << "] version [" << message_header.version << "]";
        return false;
    }
    if (message_header.body_size > 0) {
        char* buffer = new char[message_header.body_size];
        memset(buffer, 0, message_header.body_size);
        if (!m_connection.Receive(buffer, message_header.body_size)) {
            ret = false;
        }
        message->UnpackBody(buffer);
        delete[] buffer;
    }
    auto caller = FindCaller(message_header.type, message_header.version);
    if (caller == nullptr) {
        delete message;
        std::cout << "ARPC: Fail to find the caller from message type:" << message_header.type << " version: " << message_header.version;
        return false;
    }
    ret = caller(message);
    delete message;
    return ret;
}

}  // namespace arpc