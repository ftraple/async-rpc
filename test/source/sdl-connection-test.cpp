#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "sdl-client-connection.hpp"
#include "sdl-server-connection.hpp"

class SdlConnectionTest : public ::testing::Test {
   protected:
    void SetUp() override {
        m_server.Listen(8001);
        EXPECT_TRUE(m_client.Connect("localhost", 8001));
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
    char send_message[]{"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"};
    char recv_message[]{"                                                                                                                                "};
    for (int i = 0; i < 8*1000*10; i++) {
        m_client.Send((const unsigned char*)&send_message, 128);
        m_server.Receive((unsigned char*)&recv_message, 128, std::chrono::milliseconds{500});
    }
    ASSERT_STREQ(send_message, recv_message);
}
