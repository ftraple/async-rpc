#ifndef ARPC_TEST_DEFINES_HPP_
#define ARPC_TEST_DEFINES_HPP_

#include <cstdint>

enum MessageType : uint16_t {
    ping,
    pong
};

#endif  /// ARPC_TEST_DEFINES_HPP_