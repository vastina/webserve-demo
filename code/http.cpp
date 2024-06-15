#include "http.hpp"

#include <iostream>

namespace vastina {

http::http( int _fd ) : fd { _fd }, parser {}, response {}, state { NOT_WORKING }, clock { new Clock() } {};

void http::process()
{
  thread_local char readbuf[BUFSIZ];
  thread_local int str_len;
  {
    std::unique_lock<std::mutex> lck( lock );
    state = STATE::PROCESSING;
  }
  while ( true ) {
    str_len = read( fd, readbuf, BUFSIZ );
    if ( str_len == 0 ) {
      {
        std::unique_lock<std::mutex> lck( lock );
        state = STATE::NORMAL_END;
      }
      return;
    } else if ( str_len == -1 ) {
      {
        if ( EAGAIN == errno || EWOULDBLOCK == errno || EINTR == errno ) {
          continue;
        }
        std::unique_lock<std::mutex> lck( lock );
        state = STATE::ERROR_END;
        std::cout << "fd: " << fd << " error: " << errno << '\n';
        // todo: send error
      }
      return;
    } else {
      readbuf[str_len] = 0;
      handleRequst( readbuf );
      return;
    }
  }
}

http::STATE http::getcallback()
{
  std::unique_lock<std::mutex> lck( lock );
  return state;
}

void http::reset()
{
  clock->reset();
  parser.reset();
  response.reset();
  state = NOT_WORKING;
}

void http::handleRequst( const char* readbuf )
{
  //auto t1 = std::chrono::high_resolution_clock::now();

  parser.autoparse( readbuf );
  response.makeresponse( parser, fd );

  //auto t2 = std::chrono::high_resolution_clock::now();
  //std::cout << "response time: " << std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count() << '\n';
}

bool http::connection_check()
{
  return clock->check_connection();
}

} // namespace vastina
