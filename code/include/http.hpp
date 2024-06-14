#ifndef _HTTP_H_
#define _HTTP_H_

#include <arpa/inet.h>
#include <mutex>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "clock.hpp"
#include "httpparse.hpp"
#include "httpresponse.hpp"

namespace vastina {

class http
{
private:
  int fd;
  httpparser parser;
  httpresponse response;

  int state;
  Clock* clock;

  std::mutex lock;

public:
  http();
  http( int _fd );

  bool connection_check();
  void reset();
  void reponse_test( const char* readbuf, char* buf );

  enum STATE
  {
    NOT_WORKING = 0,
    PROCESSING,
    NORMAL_END,
    ERROR_END
  };
  void process();
  int getcallback();
};

} // namespace vastina

#endif // _HTTP_H_