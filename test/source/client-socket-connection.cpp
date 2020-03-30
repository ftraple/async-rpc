#include "log.hpp"
#include "client-socket-connection.hpp"

ClientSocketConnection::ClientSocketConnection() {
}

ClientSocketConnection::~ClientSocketConnection() {
    Disconnect();    
}

bool ClientSocketConnection::Connect(const std::string& host, uint16_t port) {
    //Create socket
	m_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (m_socket == -1) {
		DGT_LOG_ERROR("SOCKET: Create socket fail: ", std::strerror(errno));
        return false;
	}    
    // Set socket send and receive timeout options
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);

    // Create the server address
    std::memset(&m_server_address, 0, sizeof(m_server_address));
    m_server_address.sin_addr.s_addr = inet_addr(host.c_str());
	m_server_address.sin_family = AF_INET;
	m_server_address.sin_port = htons(port);    

    // Connect to the server
	if (connect(m_socket, (struct sockaddr*)&m_server_address , sizeof(m_server_address)) < 0) {
		DGT_LOG_ERROR("SOCKET Connection fail [", errno, "]: ", std::strerror(errno));
		return false;
	}
    m_is_connected = true;
    return true;
}

void ClientSocketConnection::Disconnect() {
    close(m_socket);
    m_is_connected = false;
}

bool ClientSocketConnection::Send(const char* data, size_t size, bool more) {
    if (!m_is_connected) {
        return false;
    }
    int send_size = send(m_socket , data, size, 0);
    if (send_size <= 0) {
        DGT_LOG_DEBUG("SOCKET SEND: Server disconnect [", errno, "]: ", std::strerror(errno));
        Disconnect();
        return false;
    }
    return true;
}

bool ClientSocketConnection::Receive(char* data, size_t size) {
    if (!m_is_connected) {
        return false;
    }
    int received_size = recv(m_socket, data , size, MSG_WAITALL);
    if (received_size <= 0) {
        DGT_LOG_DEBUG("SOCKET RECEIVE: Server disconnect [", errno ,"].");
        Disconnect();
        return false;
    }
    return true;
}
