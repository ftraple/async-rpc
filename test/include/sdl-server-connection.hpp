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

    bool Listen(uint16_t port);

    bool Accept();

    void Disconnect();

    bool Send(const char* data, size_t size, bool more) override;

    bool Receive(char* data, size_t size) override;

    private:

    TCPsocket m_listen_socket;
    TCPsocket m_socket;
};

#endif // SDL_SERVER_CONNECTION_HPP_