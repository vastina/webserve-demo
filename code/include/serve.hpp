#ifndef _SERVE_SOCK_H_
#define _SERVE_SOCK_H_

#include <memory>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <unordered_map>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <queue>

#include "loger.hpp"
#include "http.hpp"
#include "buffer.hpp"
#include "epoll.hpp"
#include "ThreadPool.hpp"

// 加了namespace vastina之后，所有线程大部分时间都会一直卡在epoll_wait
// 往threadpool里面加入新任务之后也不起任何作用
// 应该与ThreadPool不在namespace里面有关

class server
{
private:
  int serversock;
  std::unique_ptr<vastina::Epoll> ep;
  bool stopflag;
  std::unordered_map<int, vastina::http*> clients;
  ThreadPool pool;

public:
  server()
    : ep { std::make_unique<vastina::Epoll>() }
    , clients {}
    , pool { ThreadPool( 16 ) }
  {}
  ~server()
  {
    for ( auto& [k, v] : clients )
      delete v;
    clients.clear();
  }
  void init();
  void setsock( int, int, int, short = 1453 );
  void closeFD( int fd );
  void run();
  void end();
};

#endif
