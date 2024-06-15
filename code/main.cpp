#include "serve.hpp"
#include <csignal>
#include <iostream>

auto test {std::make_unique<server>()};
void make_end()
{
  test->end();
  std::exit(0);
}
int main( int argv, const char* argc[] )
{
  ::signal(SIGINT,  [](int){
    std::cout << "SIGINT" << '\n';
    make_end();});
  ::signal(SIGKILL, [](int){
    std::cout << "SIGKILL" << '\n';
    make_end();});
  ::signal(SIGQUIT, [](int){
    std::cout << "SIGQUIT" << '\n';
    make_end();
  });

  test->setsock( AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0, argv > 1 ? atoi( argc[1] ) : 5678 ); // atoi(argc[1])
  test->init();
  test->run();
  test->end();

  return 0;
}