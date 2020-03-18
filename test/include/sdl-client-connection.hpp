#ifndef SDL_CLIENT_CONNECTION_HPP_
#define SDL_CLIENT_CONNECTION_HPP_

#include <string>
#include <chrono>
#include <cstdint>
#include <SDL2/SDL_net.h>
#include <arpc/i-connection.hpp>

class SdlClientConnection : public arpc::IConnection {
    public:

    SdlClientConnection();

    ~SdlClientConnection();

    bool Connect(const std::string& host, uint16_t port);

    void Disconnect();

    bool Send(const char* data, size_t size, bool more) override;

    bool Receive(char* data, size_t size) override;

    private:

    TCPsocket m_socket;
    bool m_is_connected{false};
};

#endif // SDL_CLIENT_CONNECTION_HPP_