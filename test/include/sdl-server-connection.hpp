#ifndef SDL_SERVER_CONNECTION_HPP_
#define SDL_SERVER_CONNECTION_HPP_

#include <string>
#include <chrono>
#include <thread>
#include <cstdint>
#include <SDL2/SDL_net.h>

#include "i-connection.hpp"

class SdlServerConnection : public arpc::IConnection {
    public:

    bool Connect(const std::string& host, uint16_t port) override;

    void Accept();

    void Disconnect() override;

    bool IsConnected() override;

    bool Send(const unsigned char* data, size_t size) override;

    bool Receive(unsigned char* data, size_t max_size, std::chrono::milliseconds) override;

    private:

    TCPsocket m_listen_socket;
    TCPsocket m_socket;
};

#endif // SDL_SERVER_CONNECTION_HPP_