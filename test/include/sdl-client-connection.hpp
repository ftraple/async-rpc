#ifndef SDL_CLIENT_CONNECTION_HPP_
#define SDL_CLIENT_CONNECTION_HPP_

#include <string>
#include <chrono>
#include <cstdint>
#include <SDL2/SDL_net.h>

#include "i-connection.hpp"

class SdlClientConnection : public arpc::IConnection {
    public:

    bool Connect(const std::string& host, uint16_t port);

    void Disconnect();

    bool Send(const char* data, size_t size, bool more) override;

    bool Receive(char* data, size_t size) override;

    private:

    TCPsocket m_client_socket;
};

#endif // SDL_CLIENT_CONNECTION_HPP_