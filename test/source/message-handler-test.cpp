#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "message-handler.hpp"
#include "protocol/ping-request.hpp"
#include "protocol/ping-response.hpp"
#include "client-socket-connection.hpp"
#include "server-socket-connection.hpp"

class MessageHandlerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        m_server.Listen(8000);
        EXPECT_TRUE(m_client.Connect("127.0.0.1", 8000));
        EXPECT_TRUE(m_server.Accept());
    }

    void TearDown() override {
        m_client.Disconnect();
        m_server.Disconnect();
    }

    ClientSocketConnection m_client;
    ServerSocketConnection m_server;
};

bool HandlePingRequest(arpc::Message* message) {
    PingRequest* request = (PingRequest*)message;
    EXPECT_TRUE(request->GetCounter() == 111);
    EXPECT_TRUE(request->GetFileName().compare("Fox Molder") == 0);
    return true;
}

bool HandlePingResponse(arpc::Message* message) {
    PingResponse* response = (PingResponse*)message;
    EXPECT_TRUE(response->GetCounter() == 222);
    return true;
}

TEST_F(MessageHandlerTest, SendReceiveTest) {

    arpc::MessageHandler server_message_handler((arpc::IConnection&)m_server);
    server_message_handler.RegisterMessage(MessageType::ping, 0, PingRequest::Create, HandlePingRequest);

    arpc::MessageHandler client_message_handler((arpc::IConnection&)m_client);
    client_message_handler.RegisterMessage(MessageType::pong, 0, PingResponse::Create, HandlePingResponse);

    // Client send ping
    PingRequest ping_request;
    ping_request.SetCounter(111);
    ping_request.SetFileName("Fox Molder");
    client_message_handler.SendMessage(ping_request);
    // Server receive a ping
     server_message_handler.ReceiveMessage();
    // // Server send pong
    PingResponse ping_response;
    ping_response.SetCounter(222);
    server_message_handler.SendMessage(ping_response);
    // Client receive a pong
    client_message_handler.ReceiveMessage();
}

