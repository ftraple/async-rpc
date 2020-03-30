#include "log.hpp"
#include "server-socket-connection.hpp"

ServerSocketConnection::ServerSocketConnection() {
}

ServerSocketConnection::~ServerSocketConnection() {
    Disconnect();
}

bool ServerSocketConnection::Listen(uint16_t port) {

    // Create listen socket
    m_listen_socket = socket(AF_INET , SOCK_STREAM, 0);
    if (m_listen_socket == -1) {
	    DGT_LOG_ERROR("SERVER SOCKET: Listen socket creation fail: [", errno,"]: ", std::strerror(errno));
        return false;
    }
    // Set socket reusable option
    int reuse = 1;
    if (setsockopt(m_listen_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) {
        DGT_LOG_ERROR("SERVER SOCKET: Set sock option SO_REUSEADDR fail [", errno,"]: ", std::strerror(errno));
    }
    #ifdef SO_REUSEPORT
    if (setsockopt(m_listen_socket, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) {
        DGT_LOG_ERROR("SERVER SOCKET: Set sock option SO_REUSEPORT fail [", errno,"]: ", std::strerror(errno));
    }
    #endif
    // Prepare the sockaddr_in structure
    m_server_address.sin_family = AF_INET;
    m_server_address.sin_addr.s_addr = INADDR_ANY;
    m_server_address.sin_port = htons(port);
    bzero(&(m_server_address.sin_zero), 8); // zero the rest of the struct
    //Bind
    if (bind(m_listen_socket, (struct sockaddr*)&m_server_address , sizeof(m_server_address)) < 0) {
    	DGT_LOG_ERROR("SERVER SOCKET: Bind listen socket fail [", errno,"]: ", std::strerror(errno));
        return false;
    }
    // Listen
	if (listen(m_listen_socket , 3) < 0) {
    	DGT_LOG_ERROR("SERVER SOCKET: Listen socket fail [", errno,"]: ", std::strerror(errno));
        return false;
    }
    // Set the listens socket as non-blocking
    int flags = fcntl(m_listen_socket, F_GETFL, 0);
    fcntl(m_listen_socket, F_SETFL, flags | O_NONBLOCK);  
    return true;
}

bool ServerSocketConnection::Accept() {
    if (m_client_connected) {
        return false;
    }
	size_t address_size = sizeof(struct sockaddr_in);
	int new_client_socket = accept(m_listen_socket, (struct sockaddr*)&m_client_address, (socklen_t*)&address_size);
	if (new_client_socket < 0) {
        if (errno != EWOULDBLOCK) {  // Check for non-blocking result
    		DGT_LOG_DEBUG("SERVER SOCKET: Accept new client connection fail [", errno,"]: ", std::strerror(errno));
        }
        return false;
    }
     DGT_LOG_DEBUG("SERVER SOCKET: New user connected.");
    // Set socket send and receive timeout options
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(new_client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    setsockopt(new_client_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);
   
    // Set the new socket as non-blocking
    m_socket = new_client_socket;
    m_client_connected = true;
    return true;
}

void ServerSocketConnection::Disconnect() {
    close(m_socket);
    m_client_connected = false;
}

bool ServerSocketConnection::Send(const char* data, size_t size, bool more) {
    if (!m_client_connected) {
        return false;
    }
    int send_size = send(m_socket , data, size, 0);
    if (send_size <= 0) {
        DGT_LOG_DEBUG("SERVER SOCKET SEND: Client disconnect [", errno,"]: ", std::strerror(errno));
        Disconnect();
        return false;
    }
    return true;
}

bool ServerSocketConnection::Receive(char* data, size_t size) {
    if (!m_client_connected) {
        return false;
    }
    int received_size = recv(m_socket, data , size, MSG_WAITALL);
    if (received_size <= 0) {
        DGT_LOG_DEBUG("SERVER SOCKET RECEIVE: Client disconnect [", errno,"]: ", std::strerror(errno));
        Disconnect();
        return false;
    }
    return true;
}
