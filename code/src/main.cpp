#include "serve.hpp"

int main(int argv, const char* argc[])
{
    // if(argv != 2){
    //     std::cout << "Usage: "<<argc[0] << " <port>\n" ;
    //     return -1;
    // }

    server* test = new server();
    test->setSocket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0, 6780); // atoi(argc[1])
    test->Init();
    test->Run();
    test->End();
    test->~server();

    return 0;
}