#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <functional>
#include <future>
#include <vector>
#include <chrono>

int clients[10005];
char buffer[BUFSIZ];
struct sockaddr_in serveraddr;
std::fstream fs;
std::function<void(int)> func;

int main(int argv, char *argc[]){
    if(argv < 2) return 1;
    
    memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(atoi(argc[1])) ;
    
        fs.open("request.txt", std::ifstream::in | std::ios::binary);
        fs.read(buffer, BUFSIZ);
        fs.close();
        std::cout << buffer <<'\n';
    fs.open("result.txt", std::ofstream::out | std::ios::binary );
    func = [](int n){
        char readbuffer[BUFSIZ];    bzero(readbuffer, sizeof(readbuffer));
        for(int i=n*100; i<(n+1)*100; ++i){
            clients[i] = socket(AF_INET, SOCK_STREAM/*|SOCK_NONBLOCK*/, 0);
            if (connect(clients[i], (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
                printf("connect error with code %d", errno);
            int count = write(clients[i], buffer, BUFSIZ);
                if(count == -1)
                    printf("write error with code %d", errno);
            count = read(clients[i], readbuffer, BUFSIZ);
                if(count == -1)
                    printf("write error with code %d", errno);
            fs.write(readbuffer, BUFSIZ);
        }
        printf("%d, ", n);
    };

    auto t1 = std::chrono::high_resolution_clock::now();

    for(int i=0; i<100; ++i){
        std::thread t{[i](){
            func(i);
        }};
        t.join();
    }

    auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> tms = t2 - t1;
    std::cout << "finished in " << tms.count() << " ms" << std::endl;

    system("rm -f test/result.txt");
    fs.close();
    return 0;
}

// 1/25/24 finished in 41083.4 ms