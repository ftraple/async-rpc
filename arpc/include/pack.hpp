#ifndef ARPC_PACK_HPP_
#define ARPC_PACK_HPP_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>

//std::cout << __PRETTY_FUNCTION__ << "\n";

namespace arpc {

template <class T>
std::string to_string_impl(T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <typename... Param>
std::string to_string_impl(std::string t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <typename... Param>
std::vector<std::string> to_string(Param&... param) {
    return {to_string_impl(param)...};
}

}  // namespace arpc

#define ARPC_MSG_PACK(args...)                                          \
    std::string PackBody() {                                            \
        auto vec = arpc::to_string(args);                               \
        for (const auto& it : vec) {                                    \
            std::cout << it << std::endl;                               \
        }                                                               \
        return {"Test Body"};                                           \
    }                                                                   \
    void UnpackBody(const unsigned char* data) {                        \
        std::cout << "UnpackBody derivided" << std::endl;               \
    }

#endif  // ARPC_PACK_HPP_