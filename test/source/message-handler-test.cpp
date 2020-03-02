#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "message-handler.hpp"
#include "protocol/ping-request.hpp"
#include "protocol/ping-response.hpp"
#include "sdl-client-connection.hpp"
#include "sdl-server-connection.hpp"

class MessageHandlerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        m_server.Listen(8000);
        EXPECT_TRUE(m_client.Connect("localhost", 8000));
        EXPECT_TRUE(m_server.Accept());
    }

    void TearDown() override {
        m_client.Disconnect();
        m_server.Disconnect();
    }

    SdlClientConnection m_client;
    SdlServerConnection m_server;
};

void HandlePingRequest(arpc::Message* message) {
    PingRequest* request = (PingRequest*)message;
    std::cout << "Ping request counter: " << request->GetCounter() << std::endl;
}

void HandlePingResponse(arpc::Message* message) {
    PingResponse* response = (PingResponse*)message;
    std::cout << "Ping response counter: " << response->GetCounter() << std::endl;
}

TEST_F(MessageHandlerTest, SendReceiveTest) {

    arpc::MessageHandler server_message_handler((arpc::IConnection&)m_server);
    server_message_handler.RegisterMessage(MessageType::ping, 0, PingRequest::Create, HandlePingRequest);

    arpc::MessageHandler client_message_handler((arpc::IConnection&)m_client);
    client_message_handler.RegisterMessage(MessageType::pong, 0, PingResponse::Create, HandlePingResponse);

    // Client send ping
    PingRequest pingRequest;
    pingRequest.SetCounter(111);
    client_message_handler.SendMessage(pingRequest);

    server_message_handler.ReceiveMessage(std::chrono::milliseconds{1000});

    // Server send pong
    PingResponse pingResponse;
    pingResponse.SetCounter(222);
    server_message_handler.SendMessage(pingResponse);

    client_message_handler.ReceiveMessage(std::chrono::milliseconds{1000});
}

