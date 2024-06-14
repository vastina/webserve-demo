#include "serve.hpp"

int main(int argv, const char *argc[]){
    // if(argv != 2){
    //     std::cout << "Usage: "<<argc[0] << " <port>\n" ;
    //     return -1;
    // }

  server* test = new server();
  test->setsock( AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0, argv > 1 ? atoi( argc[1] ) : 5678 ); // atoi(argc[1])
  test->init();
  test->run();
  test->end();
  test->~server();

    return 0;
}