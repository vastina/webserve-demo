#ifndef _VAS_CONFIG_H_
#define _VAS_CONFIG_H_

#include <cstdint>

namespace vastina {

using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;

namespace config {

extern short port;
constexpr static int epoll_maxevents = 512;
constexpr static i64 connection_timeout = 10;
constexpr static int epoll_timeout = -1;

};

}

#endif