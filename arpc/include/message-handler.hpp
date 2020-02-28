#include <chrono>
#include <functional>
#include <optional>
#include <map>
#include <tuple>
#include "i-connection.hpp"
#include "message.hpp"

namespace arpc {

class MessageHandler {
   public:
    MessageHandler(IConnection& connection);

    void RegisterMessage(int type, int version,
                         std::function<Message()> create_message_function,
                         std::function<void(const Message&)> caller);

    void ReceiveMessage(std::chrono::milliseconds timeout);

    void SendMessage(Message& message);

   private:
    IConnection& m_connection;
    std::map<std::tuple<uint16_t, uint16_t>, std::function<Message(void)>> m_create_message_list;
    std::map<std::tuple<uint16_t, uint16_t>, std::function<void(const Message&)>> m_caller_list;

    std::optional<Message> CreateMessage(uint16_t type, uint16_t version);

    std::function<void(const Message&)> FindCaller(uint16_t type, uint16_t version);
    
};

}  // namespace arpc
