#include <cstdint>
#include "message.hpp"

class PingResponse : public arpc::Message {

    public:

    PingResponse() : Message(MessageType::pong, 0) {}

    static Message Create() {return PingResponse();}

    int SetCounter(int counter) {m_counter = counter;}

    int GetCounter() const {return m_counter;}

    private:

    int m_counter;

    // MSG_PACK(m_counter);
};