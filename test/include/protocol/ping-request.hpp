#include <cstdint>
#include "message.hpp"
#include "message-type.hpp"

class PingRequest : public arpc::Message {

    public:

    PingRequest() : Message(MessageType::ping, 0) {}

    static Message* Create() {return new PingRequest();}

    int SetCounter(int counter) {m_counter = counter;}

    int GetCounter() const {return m_counter;}

    private:

    int m_counter{0};

    ARPC_MSG_PACK(m_counter);
};