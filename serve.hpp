#ifndef _SERVE_SOCK_H_
#define _SERVE_SOCK_H_


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

#include "tools.h"
#include "log/loger.hpp"
#include "http/http.hpp"
#include "buffer/buffer.hpp"
#include "epoll/epoll.hpp"
#include "epoll/listener.hpp"

class server{
private:
    int serversock;

    vastina::Epoll *ep;

    char readbuffer[BUFSIZE], sendbuffer[BUFSIZE] ; 
    //Buffer *buffer;
    //to be finished as a class or struct
    bool stopflag;

    std::unordered_map<int, vastina::http*> clients;

public:
    server(size_t _maxevents = 50) :
        clients{ } {
        ep = new vastina::Epoll(_maxevents);
        //buffer = new Buffer(); 
        }
    ~server(){
        //close(serversock);
    }
    void init();
    void setsock(int af,int type,int protocol, short port = 1453);
    void closeFD(int fd);
    void run();
    void end();
};

void server::setsock(int af,int type,int protocol, short port){
    serversock = socket(af, type, protocol);
    int reuse = 1, nodelay = 1;
    setsockopt(serversock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    setsockopt(serversock, IPPROTO_TCP, 1, &nodelay, sizeof(nodelay));
    int flag = fcntl(serversock, F_GETFL, 0);
    fcntl(serversock, F_SETFL, flag | O_NONBLOCK);

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = af;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port) ;
    if(bind(serversock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        printf("bind fail, error code: %d\n", errno);
    }
    if(::listen(serversock, 100) == -1)
    {
        printf("listen fail, error code: %d\n", errno);
    }
    
}

void server::init(){
    ep->init(serversock);
    stopflag = true;
    vastina::cachetree::getInstance().init_read();
}

void server::closeFD(int fd){
    ep->epoll_del(fd);
    close(fd);
    //delete clients[fd];
    clients.erase(fd);
}


void server::run(){

    std::cout <<"Enter q to quit\n";
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    while (stopflag)
    {
        int event_cnt = ep->Epoll_wait();
        if(event_cnt == -1)
        {

        }
        for(auto i=0; i<event_cnt; ++i)
        {
            if(ep->getevent(i) & EPOLLIN)
            {
                if(ep->getfd(i) == serversock)
                {
                    int clntsock = accept(serversock, (struct sockaddr *)&addr, &len);
                    ep->epoll_add(clntsock);
                    if(!clients[clntsock]) clients[clntsock] = new vastina::http();
                    //else clients[clntsock]->reset(); todo
                } 
                else
                {
                    int str_len = read(ep->getfd(i), readbuffer, sizeof(readbuffer));
                    if(str_len == 0)
                    {
                        static int count = 0;
                        printf("count: %d, fd: %d\n", ++count, ep->getfd(i));

                        int sock = ep->getfd(i);
                        //if(clients[sock]->connection_check())
                            closeFD(sock);
                    }
                    else
                    {
                        int sock = ep->getfd(i);

                        auto t1 = std::chrono::system_clock::now();

                        clients[sock]->getreponse_test(readbuffer, sendbuffer);
                        write(sock, sendbuffer, sizeof(sendbuffer));

                        auto t2 = std::chrono::system_clock::now();
                        std::cout << "cost time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds\n";
                    }
                }
            }
            if(ep->getevent(i) & EPOLLHUP)
            {
                int sock = ep->getfd(i);
                delete clients[sock];
            }

        }
        //stopflag = ep->stdincheck();
    }
}

void server::end(){
    close(serversock);
    for(auto &client:clients){ delete client.second; }
    ep->~Epoll();
    delete ep;
}


#endif