#include <cstdint>
#include "message.hpp"
#include "message-type.hpp"

class PingResponse : public arpc::Message {
   public:
    PingResponse() : Message(MessageType::pong, 0) {}

    static PingResponse* Create() { return new PingResponse(); }

    void SetCounter(int counter) { m_counter = counter; }

    int GetCounter() const { return m_counter; }

   private:
    int m_counter{0};

   public:
    ARPC_MSG_PACK(m_counter);
};