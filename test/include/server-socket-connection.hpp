#ifndef DGT_FILE_TRANSFER_SERVER_SOCKET_CONNECTION_HPP_
#define DGT_FILE_TRANSFER_SERVER_SOCKET_CONNECTION_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpc/i-connection.hpp>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>

class ServerSocketConnection : public arpc::IConnection {
   public:
    ServerSocketConnection();

    ~ServerSocketConnection();

    bool Listen(uint16_t port);

    bool Accept();

    void Disconnect();

    bool Send(const char* data, size_t size, bool more) override;

    bool Receive(char* data, size_t size) override;

   private:
    int m_listen_socket;
    int m_socket;
    struct sockaddr_in m_server_address;
    struct sockaddr_in m_client_address;
    bool m_client_connected{false};
};

#endif  // DGT_FILE_TRANSFER_SERVER_SOCKET_CONNECTION_HPP_