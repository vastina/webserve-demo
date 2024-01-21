#include "serve.hpp"

int main(){
    server *test = new server();
    test->setsock(AF_INET, SOCK_STREAM, IPPROTO_TCP, 2345);
    test->init();
    test->run();
    test->end();
    test->~server();
}