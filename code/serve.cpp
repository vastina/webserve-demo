#include "serve.hpp"

void server::setsock( int af, int type, int protocol, short port )
{
  serversock = socket( af, type, protocol );
  int reuse = 1, nodelay = 1;
  setsockopt( serversock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
  setsockopt( serversock, IPPROTO_TCP, 1, &nodelay, sizeof( nodelay ) );
  int flag = fcntl( serversock, F_GETFL, 0 );
  fcntl( serversock, F_SETFL, flag | O_NONBLOCK );

  struct sockaddr_in serveraddr;
  memset( &serveraddr, 0, sizeof( serveraddr ) );
  serveraddr.sin_family = af;
  serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
  serveraddr.sin_port = htons( port );
  if ( bind( serversock, (struct sockaddr*)&serveraddr, sizeof( serveraddr ) ) == -1 ) {
    printf( "bind fail, error code: %d\n", errno );
  }
  if ( ::listen( serversock, 100 ) == -1 ) {
    printf( "listen fail, error code: %d\n", errno );
  }
}

void server::init()
{
  ep->init( serversock );
  stopflag = true;
  vastina::cachetree::getInstance().init_read();
}

void server::closeFD( int fd )
{
  ep->epoll_del( fd );
  close( fd );
  // delete clients[fd];
  clients.erase( fd );
}

void server::run()
{

  std::cout << "Enter q to quit\n";
  struct sockaddr_in addr;
  socklen_t len = sizeof( addr );

  while ( stopflag ) {
    int event_cnt = ep->Epoll_wait();
    if ( event_cnt == -1 ) { // until now, I haven't met this situation
      printf( "epoll_wait fail, error code: %d\n", errno );
    }
    for ( auto i = 0; i < event_cnt; ++i ) {
      if ( ep->getevent( i ) & EPOLLIN ) {
        if ( ep->getfd( i ) == serversock ) {
          int clntsock = accept( serversock, (struct sockaddr*)&addr, &len );
          ep->epoll_add( clntsock );

          if ( !clients[clntsock] )
            clients[clntsock] = new vastina::http( clntsock );
          else
            clients[clntsock]->reset();

          std::cout << "fd: " << clntsock << '\n';

          pool.enqueue( [this, clntsock]() { clients[clntsock]->process(); } );
        } else {
          int clntsock = ep->getfd( i );
          int callback = clients[clntsock]->getcallback();
          std::cout << "fd: " << clntsock << " callback: " << callback << '\n';
          if ( callback == vastina::http::STATE::NORMAL_END ) {
            if ( !clients[clntsock]->connection_check() )
              closeFD( clntsock );
          } else if ( callback == vastina::http::STATE::ERROR_END ) {
            if ( errno != EAGAIN && errno != EWOULDBLOCK )
              closeFD( clntsock );
            // todo:log it
          }
        }
      }
      if ( ep->getevent( i ) & EPOLLHUP ) {
        int sock = ep->getfd( i );
        delete clients[sock];
      }
    }
    stopflag = ep->stdincheck();
  }
}

void server::end()
{
  close( serversock );
  clients.clear();
  ep->~Epoll();
}