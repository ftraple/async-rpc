
#include "sdl-client-connection.hpp"

bool SdlClientConnection::Connect(const std::string& host, uint16_t port) {
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "ER: SDLNet_Init: %s\n", SDLNet_GetError());
        return false;
    }
    IPaddress ip_address;
    if (SDLNet_ResolveHost(&ip_address, host.c_str(), port) == -1) {
      printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    m_client_socket = SDLNet_TCP_Open(&ip_address);
    if (!m_client_socket) {
      printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return false;
    }
    return true;
}

void SdlClientConnection::Disconnect() {
    SDLNet_TCP_Close(m_client_socket);    
    SDLNet_Quit();
}

bool SdlClientConnection::Send(const unsigned char* data, size_t size) {
    SDLNet_TCP_Send(m_client_socket, data, size);
    return true;
}

bool SdlClientConnection::Receive(unsigned char* data, size_t max_size, std::chrono::milliseconds) {
    int received_size = SDLNet_TCP_Recv(m_client_socket, data, max_size);
    if (received_size != max_size) {
        return false;
    }
    return true;
}
