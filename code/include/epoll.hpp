#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "config.hpp"

namespace vastina {

class Epoll
{

private:
  epoll_event* ep_events;
  epoll_event event;
  int epfd;
  size_t maxevents;

  struct _stdincheck
  {
    epoll_event event[1];
    int epstdin;
  } stdinchecker;

public:
  Epoll( size_t _maxevents = config::epoll_maxevents );
  ~Epoll();

  void init( int serversock );
  int Epoll_wait();
  uint32_t getevent( int index );
  void epoll_add( int fd );
  void epoll_del( int index );
  int getfd( int index );

  bool stdincheck();
};

} // namespace vastina

#endif