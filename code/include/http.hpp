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
public:
  enum STATE
  {
    NOT_WORKING = 0,
    PROCESSING,
    NORMAL_END,
    ERROR_END
  };

private:
  int fd;
  httpparser parser;
  httpresponse response;

  STATE state;
  Clock* clock;

  std::mutex lock;

public:
  http( int _fd );

  bool connection_check();
  void reset();
  void handleRequst( const char* readbuf );
  void process();
  STATE getcallback();
};

} // namespace vastina

#endif // _HTTP_H_