#ifndef SDL_CLIENT_CONNECTION_HPP_
#define SDL_CLIENT_CONNECTION_HPP_

#include <string>
#include <chrono>
#include <cstdint>
#include <SDL2/SDL_net.h>

#include "i-send-receive.hpp"

class SdlClientConnection : public arpc::ISendReceive {
    public:

    bool Connect(const std::string& host, uint16_t port);

    void Disconnect();

    bool Send(const unsigned char* data, size_t size);

    bool Receive(unsigned char* data, size_t max_size, std::chrono::milliseconds);

    private:

    TCPsocket m_client_socket;
};

#endif // SDL_CLIENT_CONNECTION_HPP_