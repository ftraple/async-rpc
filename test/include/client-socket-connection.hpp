#ifndef DGT_FILE_TRANSFER_CLIENT_SOCKET_CONNECTION_HPP_
#define DGT_FILE_TRANSFER_CLIENT_SOCKET_CONNECTION_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpc/i-connection.hpp>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>

class ClientSocketConnection : public arpc::IConnection {
   public:
    ClientSocketConnection();

    ~ClientSocketConnection();

    bool Connect(const std::string& host, uint16_t port);

    void Disconnect();

    bool Send(const char* data, size_t size, bool more) override;

    bool Receive(char* data, size_t size) override;

   private:
    int m_socket;
    struct sockaddr_in m_server_address;
    bool m_is_connected{false};
};

#endif  // DGT_FILE_TRANSFER_CLIENT_SOCKET_CONNECTION_HPP_