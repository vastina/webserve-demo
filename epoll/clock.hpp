#ifndef _COLOCK_H_
#define _COLOCK_H_

#include <chrono>
#include <iostream>

namespace vastina {

constexpr int TIMEOUT = 10;

struct Clock {
	std::chrono::time_point<std::chrono::system_clock> start;
	bool not_connected;
	bool check_connection();
};

} // namespace vastina

#endif