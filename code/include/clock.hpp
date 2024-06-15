#ifndef _COLOCK_H_
#define _COLOCK_H_

#include <chrono>

namespace vastina {

constexpr int TIMEOUT = 10;

struct Clock
{
  std::chrono::time_point<std::chrono::system_clock> start;
  bool not_connected {false};
  bool check_connection();
  void reset();
};

} // namespace vastina

#endif