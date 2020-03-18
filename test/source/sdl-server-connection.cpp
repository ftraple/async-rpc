
#include "sdl-server-connection.hpp"

SdlServerConnection::SdlServerConnection() {
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "ER: SDLNet_Init: %s\n", SDLNet_GetError());
    }
}

SdlServerConnection::~SdlServerConnection() {
    Disconnect();
    SDLNet_Quit();
    free(m_listen_socket);
}

bool SdlServerConnection::Listen(uint16_t port) {
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
    TCPsocket socket = SDLNet_TCP_Accept(m_listen_socket);
    if (!socket) {
        return false;
    }
    if (!m_client_connected) {
        Disconnect();
    }
    m_socket = socket;
    m_client_connected = true;
    return true;
}

void SdlServerConnection::Disconnect() {
    if (m_client_connected) {
        SDLNet_TCP_Close(m_socket);
    }
    m_client_connected = false;
}

bool SdlServerConnection::Send(const char* data, size_t size, bool more) {
    if (!m_client_connected) {
        return false;
    }
    int send_size = SDLNet_TCP_Send(m_socket, data, size);
    if (send_size != size) {
        return false;
    }
    return true;
}

bool SdlServerConnection::Receive(char* data, size_t size) {
    if (!m_client_connected) {
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
