#ifndef SDL_CLIENT_CONNECTION_HPP_
#define SDL_CLIENT_CONNECTION_HPP_

#include <string>
#include <chrono>
#include <cstdint>
#include <SDL2/SDL_net.h>

#include "i-connection.hpp"

class SdlClientConnection : public arpc::IConnection {
    public:

    bool Connect(const std::string& host, uint16_t port) override;

    void Disconnect() override;

    bool IsConnected() override;

    bool Send(const unsigned char* data, size_t size) override;

    bool Receive(unsigned char* data, size_t max_size, std::chrono::milliseconds) override;

    private:

    TCPsocket m_client_socket;
};

#endif // SDL_CLIENT_CONNECTION_HPP_