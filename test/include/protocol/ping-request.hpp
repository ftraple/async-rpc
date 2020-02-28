#include <cstdint>
#include "message.hpp"
#include "defines.hpp"

class PingRequest : public arpc::Message {

    public:

    PingRequest() : Message(MessageType::ping, 0) {}

    static Message Create() {return PingRequest();}

    int SetCounter(int counter) {m_counter = counter;}

    int GetCounter() const {return m_counter;}

    private:

    int m_counter;

    // MSG_PACK(m_counter);
};