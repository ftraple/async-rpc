#ifndef ARPC_TEST_MESSAGE_TYPE_HPP_
#define ARPC_TEST_MESSAGE_TYPE_HPP_

#include <cstdint>

enum MessageType : uint16_t {
    ping = 10,
    pong
};

#endif  /// ARPC_TEST_MESSAGE_TYPE_HPP_