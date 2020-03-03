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

    uint16_t m_value1{1};
    float m_value2{2.5};
    std::string m_value3{"bla"};

    ARPC_MSG_PACK(m_value1, m_value2, m_value3);
};