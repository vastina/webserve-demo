#include "serve.hpp"
#include "ThreadPool.hpp"
#include "config.hpp"

void server::setsock( int domain, int type, int protocol, short port )
{
  serversock = socket( domain, type, protocol );
  int reuse = 1;//, nodelay = 1;
  setsockopt( serversock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
  //setsockopt( serversock, IPPROTO_TCP, 1, &nodelay, sizeof( nodelay ) );
  int flag = fcntl( serversock, F_GETFL, 0 );
  fcntl( serversock, F_SETFL, flag | O_NONBLOCK );

  struct sockaddr_in serveraddr;
  memset( &serveraddr, 0, sizeof( serveraddr ) );
  serveraddr.sin_family = domain;
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
  // clients.erase( fd );
}

void server::run()
{

  // std::cout << "Enter q to quit\n";
  struct sockaddr_in addr;
  socklen_t len = sizeof( addr );
  bzero( &addr, len );

  while ( stopflag ) {
    int event_cnt = ep->Epoll_wait();
    if ( event_cnt == -1 ) { // until now, I haven't met this situation
      printf( "epoll_wait fail, error code: %d\n", errno );
    }
    for ( auto i = 0; i < event_cnt && i < vastina::config::epoll_maxevents; ++i ) {
      int sock = ep->getfd( i );
      auto events = ep->getevent( i );
       if ( sock == serversock ) {
        int clntsock = accept( serversock, (struct sockaddr*)&addr, &len );
        if( clntsock < 0 ) continue;
        else if( clntsock >= 65536 ) {
          ::close( clntsock );
          continue;
        }
        ep->epoll_add( clntsock );
        if ( !clients.contains( clntsock ) )
          clients[clntsock] = new vastina::http( clntsock );
        else
          clients[clntsock]->reset();

      } else if ( events & EPOLLIN ) {
        pool.enqueue( [this, sock]() {
          clients[sock]->process();
          closeFD(sock);
        } );
        // std::cout << '(' << sock << " read) ";
      } else if ( events & EPOLLOUT ) {
        // std::cout << '(' << sock << " write) ";
      } else if ( events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR) ) {
        // std::cout << '(' << sock << " close) ";
        // closeFD( sock );
      } else {

      }
    }
  }
}

void server::end()
{
  stopflag = false;
  close( serversock );
}