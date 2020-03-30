#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "client-socket-connection.hpp"
#include "server-socket-connection.hpp"

class SdlConnectionTest : public ::testing::Test {
   protected:
    void SetUp() override {
        m_server.Listen(8001);
        EXPECT_TRUE(m_client.Connect("127.0.0.1", 8001));
        EXPECT_TRUE(m_server.Accept());
    }

    void TearDown() override {
        m_client.Disconnect();
        m_server.Disconnect();
    }

    ClientSocketConnection m_client;
    ServerSocketConnection m_server;
};

TEST_F(SdlConnectionTest, SendReceiveTest) {
    char send_message[]{"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"};
    char recv_message[]{"                                                                                                                                "};
    for (int i = 0; i < 8*1000*10; i++) {
        m_client.Send((const char*)&send_message, 128, false);
        m_server.Receive((char*)&recv_message, 128);
    }
    ASSERT_STREQ(send_message, recv_message);
}
