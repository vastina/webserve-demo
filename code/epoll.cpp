#include "epoll.hpp"
#include "config.hpp"

#include <unistd.h>

namespace vastina {

Epoll::Epoll( size_t _maxevents ) : epfd { epoll_create( _maxevents ) }, maxevents { _maxevents }
{
  ep_events = new epoll_event[_maxevents];
}

Epoll::~Epoll()
{
  close( epfd );
  delete[] ep_events;
}

void Epoll::init( int serversock )
{
  event.data.fd = serversock;
  event.events = EPOLLIN | EPOLLET;
  epoll_ctl( epfd, EPOLL_CTL_ADD, serversock, &event );
}

int Epoll::Epoll_wait()
{
  return epoll_wait( epfd, ep_events, maxevents, config::epoll_timeout );
}

uint32_t Epoll::getevent( int index )
{
  return ep_events[index].events;
}

void Epoll::epoll_add( int fd )
{
  int flag = fcntl( fd, F_GETFL, 0 );
  fcntl( fd, F_SETFL, flag | SOCK_NONBLOCK );

  event.events = EPOLLIN | EPOLLET;
  event.data.fd = fd;
  epoll_ctl( epfd, EPOLL_CTL_ADD, fd, &event );
}

void Epoll::epoll_del( int fd )
{
  event = { STDIN_FILENO, { .fd = STDIN_FILENO } };

  epoll_ctl( epfd, EPOLL_CTL_DEL, fd, &event );
}

int Epoll::getfd( int index )
{
  return ep_events[index].data.fd;
}

} // namespace vastina