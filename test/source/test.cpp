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
        m_server.Listen(8000);
        EXPECT_TRUE(m_client.Connect("127.0.0.1", 8000));
        EXPECT_TRUE(m_server.Accept());
    }

    void TearDown() override {
        m_client.Disconnect();
        m_server.Disconnect();
    }

    SdlClientConnection m_client;
    SdlServerConnection m_server;
};

TEST_F(SdlConnectionTest, SendReceiveTest) {
  char    send_message[]{"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"};
  char receive_message[]{"                                                                                                                                "};
  for (int i = 0; i < 1; i++) {
    m_client.Send((const unsigned char*)&send_message, sizeof(send_message));
    m_server.Receive((unsigned char*)&receive_message, sizeof(send_message), std::chrono::milliseconds{500});
  }
  ASSERT_STREQ(send_message, receive_message);
}
