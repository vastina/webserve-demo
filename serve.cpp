#include "serve.hpp"

int main(int argv, const char *argc[]){
    if(argv != 2) return 1;

    server *test = new server();
    test->setsock(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0, atoi(argc[1]));
    test->init();
    test->run();
    test->end();
    test->~server();

    return 0;
}