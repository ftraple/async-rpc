#ifndef ARPC_MESSAGE_HANDLER_HPP_
#define ARPC_MESSAGE_HANDLER_HPP_

#include <chrono>
#include <functional>
#include <memory>
#include <map>
#include <tuple>
#include <exception>
#include <string>
#include <sstream>
#include "i-connection.hpp"
#include "message.hpp"

namespace arpc {

typedef std::function<Message*(void)> CreateMessageFunction;
typedef std::function<bool(Message*)> CallerFunction;

class MessageHandler {
   public:
    MessageHandler(IConnection& connection);

    void RegisterMessage(int type, int version,
                         CreateMessageFunction create_message_function,
                         CallerFunction caller);

    bool ReceiveMessage();

    template <typename T>
    bool SendMessage(T& message) {
        size_t bufferSize = message.PackBodySize();
        if (bufferSize == 0) {
            message.SetBodySize(bufferSize);
            if (!m_connection.Send(message.PackHeader(), message.PackHeaderSize(), false)) {
                return false;
            }
        } else {
            message.SetBodySize(bufferSize);
            if (!m_connection.Send(message.PackHeader(), message.PackHeaderSize(), true)) {
                return false;
            }
            char* buffer = new char[bufferSize];
            memset(buffer, 0, bufferSize);
            message.PackBody(buffer);
            if (!m_connection.Send(buffer, bufferSize, false)) {
                delete[] buffer;
                return false;
            }
            delete[] buffer;
        }
        return true;
    }

   private:
    IConnection& m_connection;
    std::map<std::tuple<uint16_t, uint16_t>, CreateMessageFunction> m_create_message_list;
    std::map<std::tuple<uint16_t, uint16_t>, CallerFunction> m_caller_list;

    Message* CreateMessage(uint16_t type, uint16_t version);

    CallerFunction FindCaller(uint16_t type, uint16_t version);
};

}  // namespace arpc

#endif  // ARPC_MESSAGE_HANDLER_HPP_