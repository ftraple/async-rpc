#include <cstdint>
#include "message.hpp"
#include "message-type.hpp"

class PingRequest : public arpc::Message {
   public:
    PingRequest() : Message(MessageType::ping, 0) {
        m_header.body_size = PackBodySize();
    }

    static PingRequest* Create() { return new PingRequest(); }

    int SetCounter(int counter) { m_counter = counter; }

    int GetCounter() const { return m_counter; }

   private:
    uint64_t m_counter{0};

   public:
    ARPC_MSG_PACK(m_counter);
};