#ifndef ARPC_PACK_HPP_
#define ARPC_PACK_HPP_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <type_traits>
#include <vector>

namespace arpc::pack {

//------------------------------------------------------------

template <class T>
size_t PackBodySizeImpl(T& t) {
    return sizeof(t);
}
template <typename... Args>
size_t PackBodySizeImpl(std::string& text) {
    return sizeof(uint16_t) + text.size();
}
template <typename... Args>
size_t PackBodySize(Args&... args) {
    std::vector<size_t> vec = {PackBodySizeImpl(args)...};
    size_t size{0};
    for (const auto& it : vec) {
        size += it;
    }
    return size;
}

//------------------------------------------------------------

template <typename T>
void PackBodyImpl(char* buffer, int& offset, T& t) {
    std::memcpy(buffer + offset, &t, sizeof(t));
    offset += sizeof(t);
}

template <typename... Args>
void PackBodyImpl(char* buffer, int& offset, std::string& text) {
    uint16_t textSize = text.length();
    std::memcpy(buffer + offset, &textSize, sizeof(textSize));
    offset += sizeof(textSize);
    std::memcpy(buffer + offset, text.data(), textSize);
    offset += textSize;
}

template <typename First, typename... Args>
void PackBodyImpl(char* buffer, int& offset, First& first, Args&... args) {
    PackBodyImpl(buffer, offset, first);
    PackBodyImpl(buffer, offset, args...);
}

template <typename... Args>
void PackBody(char* buffer, Args&... args) {
    int offset{0};
    PackBodyImpl(buffer, offset, args...);
}

//------------------------------------------------------------

template <typename T>
void UnpackBodyImpl(const char* buffer, int& offset, T& t) {
    std::memcpy(&t, buffer + offset, sizeof(t));
    offset += sizeof(t);
}

template <typename... Args>
void UnpackBodyImpl(const char* buffer, int& offset, std::string& text) {
    uint16_t textSize{0};
    std::memcpy(&textSize, buffer + offset, sizeof(textSize));
    offset += sizeof(textSize);
    text.resize(textSize);
    std::memcpy(text.data(), buffer + offset, textSize);
    offset += textSize;
}

template <typename First, typename... Args>
void UnpackBodyImpl(const char* buffer, int& offset, First& first, Args&... args) {
    UnpackBodyImpl(buffer, offset, first);
    UnpackBodyImpl(buffer, offset, args...);
}

template <typename... Args>
void UnpackBody(const char* buffer, Args&... args) {
    int offset{0};
    UnpackBodyImpl(buffer, offset, args...);
}

//------------------------------------------------------------

}  // namespace arpc::pack

#define ARPC_MSG_PACK(args...)                 \
    size_t PackBodySize() {                    \
        return arpc::pack::PackBodySize(args); \
    }                                          \
    void PackBody(char* buffer) {              \
        arpc::pack::PackBody(buffer, args);    \
    }                                          \
    void UnpackBody(const char* buffer) {      \
        arpc::pack::UnpackBody(buffer, args);  \
    }

#endif  // ARPC_PACK_HPP_
