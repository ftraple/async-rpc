#include <iostream>
#include "sdl-client-connection.hpp"

SdlClientConnection::SdlClientConnection() {
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "ER: SDLNet_Init: %s\n", SDLNet_GetError());
    }
}

SdlClientConnection::~SdlClientConnection() {
    Disconnect();    
    SDLNet_Quit();
}

bool SdlClientConnection::Connect(const std::string& host, uint16_t port) {
    IPaddress ip_address;
    if (SDLNet_ResolveHost(&ip_address, host.c_str(), port) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    m_socket = SDLNet_TCP_Open(&ip_address);
    if (!m_socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return false;
    }
    m_is_connected = true;
    return true;
}

void SdlClientConnection::Disconnect() {
    if (m_is_connected) {
        SDLNet_TCP_Close(m_socket);
    }
    m_is_connected = false;
}

bool SdlClientConnection::Send(const char* data, size_t size, bool more) {
    if (!m_is_connected) {
        return false;
    }
    int send_size = SDLNet_TCP_Send(m_socket, data, size);
    if (send_size != size) {
        return false;
    }
    return true;
}

bool SdlClientConnection::Receive(char* data, size_t size) {
    if (!m_is_connected) {
        return false;
    }
    int total_size = 0;
    do {
        int received_size = SDLNet_TCP_Recv(m_socket, data + total_size, size-total_size);
        if (received_size <= 0) {
            Disconnect();
            return false;
        }
        total_size += received_size;
    } while (total_size < size);
    return true;
}
