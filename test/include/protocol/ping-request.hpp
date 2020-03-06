#include <cstdint>
#include <string>
#include "message.hpp"
#include "message-type.hpp"

class PingRequest : public arpc::Message {
   public:
    PingRequest() : Message(MessageType::ping, 0) {}

    static PingRequest* Create() { return new PingRequest(); }

    int SetCounter(int counter) { m_counter = counter; }

    int GetCounter() const { return m_counter; }

    void SetFileName(const std::string& file_name) { m_file_name = file_name; }

    std::string GetFileName() const { return m_file_name; }

   private:
    uint64_t m_counter{0};
    std::string m_file_name{""};

   public:
    ARPC_MSG_PACK(m_counter, m_file_name);
};