#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include "sdl-client-connection.hpp"
#include "sdl-server-connection.hpp"
#include "ping-request.hpp"
#include "ping-response.hpp"

class SdlConnectionTest : public ::testing::Test {
   protected:
    void SetUp() override {
        m_server.Connect("*", 8000);
        m_client.Connect("127.0.0.1", 8000);
        m_server.Accept();
    }

    void TearDown() override {
        m_client.Disconnect();
        m_server.Disconnect();
    }

    SdlClientConnection m_client;
    SdlServerConnection m_server;
};

TEST_F(SdlConnectionTest, SendReceiveTest) {
  char send_message[]{"HELLO"};
  char receive_message[]{"     "};
  m_client.Send((const unsigned char*)&send_message, sizeof(send_message));
  m_server.Receive((unsigned char*)&receive_message, sizeof(send_message), std::chrono::milliseconds{500});
  ASSERT_STREQ(send_message, receive_message);
}
