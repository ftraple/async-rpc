
#include "sdl-server-connection.hpp"

bool SdlServerConnection::Listen(uint16_t port) {
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "ER: SDLNet_Init: %s\n", SDLNet_GetError());
        return false;
    }
    IPaddress ip_address;
    if (SDLNet_ResolveHost(&ip_address, NULL, port) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    m_listen_socket = SDLNet_TCP_Open(&ip_address);
    if (!m_listen_socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return false;
    }
    return true;
}

bool SdlServerConnection::Accept() {
    m_socket = SDLNet_TCP_Accept(m_listen_socket);
    if (!m_socket) {
        return false;
    }
    return true;
}

void SdlServerConnection::Disconnect() {
    SDLNet_TCP_Close(m_socket);
    SDLNet_Quit();
}

bool SdlServerConnection::Send(const unsigned char* data, size_t size) {
    SDLNet_TCP_Send(m_socket, data, size);
    return true;
}

bool SdlServerConnection::Receive(unsigned char* data, size_t max_size, std::chrono::milliseconds) {
    int received_size = SDLNet_TCP_Recv(m_socket, data, max_size);
    if (received_size != max_size) {
        return false;
    }
    return true;
}
