#include <cstdint>
#include "message.hpp"

class PingRequest : public arpc::Message {

    public:

    PingRequest(int type, int version) : Message(type, version) {}

    int SetCounter(int counter) {m_counter = counter;}

    int GetCounter() const {return m_counter;}

    private:

    int m_counter;

    // MSG_PACK(m_counter);
};