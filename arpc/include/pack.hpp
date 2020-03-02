#ifndef ARPC_PACK_HPP_
#define ARPC_PACK_HPP_

namespace arpc {

#define ARPC_MSG_PACK(args...)     \
    unsigned char* PackBody() {    \
        return nullptr;            \
    }                              \
    size_t PackBodySize() {        \
        return 0;                  \
    }                              \
    void UnpackBody(size_t size) { \
        return;                    \
    }

}  // namespace arpc

#endif  // ARPC_PACK_HPP_