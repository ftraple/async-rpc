#include <chrono>
#include <functional>
#include <map>
#include <tuple>
#include "i-connection.hpp"
#include "message.hpp"

namespace arpc {

class MessageHandler {
   public:
    MessageHandler(IConnection& connection);

    void RegisterMessage(int type, int version,
                         std::function<void(uint16_t, uint16_t)> create_message,
                         std::function<void(const Message&)> caller);

    void ReceiveMessage(std::chrono::milliseconds timeout);

    void SendMessage(const Message& message);

   private:
    IConnection& m_connection;
    std::map<std::tuple<uint16_t, uint16_t>, std::function<void(const Message&)>> m_caller_list;

    Message CreateMessage(uint16_t type, uint16_t version);
};

}  // namespace arpc
