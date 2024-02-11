#ifndef _HTTP_H_
#define _HTTP_H_

#include <arpa/inet.h>
#include <condition_variable>
#include <cstring>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <mutex>
#include <set>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <thread>

#include "../epoll/clock.hpp"
#include "httpparse.hpp"
#include "httpresponse.hpp"

namespace vastina {

class http {
  private:
	int fd;
	httpparser parser;
	httpresponse response;

	Clock *clock;
	int state;
	// bool stopflag;
	std::mutex lock;
	// std::condition_variable newMessage;

  public:
	http()
		: fd{-1}, parser{}, response{httpresponse()},
		  // stopflag{true},
		  state{NOT_WORKING} {
		clock = new Clock();
	};
	http(int _fd)
		: fd{_fd}, parser{}, response{},
		  // stopflag{true},
		  state{NOT_WORKING} {
		clock = new Clock();
	};

	bool connection_check();
	void reset();
	void reponse_test(const char *readbuf, char *buf);

	enum STATE { NOT_WORKING = 0, PROCESSING, NORMAL_END, ERROR_END };
	void process();
	int getcallback();
};

} // namespace vastina

#endif // _HTTP_H_