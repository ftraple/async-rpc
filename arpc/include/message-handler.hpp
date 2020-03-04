#include <chrono>
#include <functional>
#include <memory>
#include <map>
#include <tuple>
#include "i-connection.hpp"
#include "message.hpp"

namespace arpc {

typedef std::function<Message*(void)> CreateMessageFunction;
typedef std::function<void(Message*)> CallerFunction;

class MessageHandler {
   public:
    MessageHandler(IConnection& connection);

    void RegisterMessage(int type, int version,
                         CreateMessageFunction create_message_function,
                         CallerFunction caller);

    void ReceiveMessage(std::chrono::milliseconds timeout);

    template<typename T>
    void SendMessage(T& message) {
        m_connection.Send(message.PackHeader(), message.PackHeaderSize());
        size_t bufferSize = message.PackBodySize();
        unsigned char* buffer = new unsigned char[bufferSize];
        memset(buffer, 0, bufferSize);
        message.PackBody(buffer);
        m_connection.Send(buffer, bufferSize);
        delete[] buffer;
    }

   private:
    IConnection& m_connection;
    std::map<std::tuple<uint16_t, uint16_t>, CreateMessageFunction> m_create_message_list;
    std::map<std::tuple<uint16_t, uint16_t>, CallerFunction> m_caller_list;

    Message* CreateMessage(uint16_t type, uint16_t version);

    CallerFunction FindCaller(uint16_t type, uint16_t version);
    
};

}  // namespace arpc
