#include "config.hpp"
#include "serve.hpp"
#include <csignal>
#include <iostream>

namespace vastina {
namespace config {
short port = 5678;
}
}
auto test { std::make_unique<server>() };
void make_end()
{
  test->end();
  std::exit( 0 );
}
int main( int argv, const char* argc[] )
{
  ::signal( SIGINT, []( int ) {
    std::cout << "SIGINT" << '\n';
    make_end();
  } );
  ::signal( SIGKILL, []( int ) {
    std::cout << "SIGKILL" << '\n';
    make_end();
  } );
  ::signal( SIGQUIT, []( int ) {
    std::cout << "SIGQUIT" << '\n';
    make_end();
  } );
  ::signal( SIGSEGV, []( int ) {
    std::cout << "SIGSEGV" << '\n';
    make_end();
  } );
  ::signal( SIGPIPE, []( int ) {} );

  vastina::config::port = argv > 1 ? atoi( argc[1] ) : 5678;
  test->setsock( AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0, vastina::config::port );
  test->init();
  test->run();
  test->end();

  return 0;
}