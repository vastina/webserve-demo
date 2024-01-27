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

#include "tools.h"
#include "log/loger.hpp"
#include "http/http.hpp"
#include "buffer/buffer.hpp"
#include "epoll/epoll.hpp"

class server{
private:
    int serversock;

    Epoll *ep;

    char readbuffer[BUFSIZE], sendbuffer[BUFSIZE] ; 
    //Buffer *buffer;
    //to be finished as a class or struct

    std::unordered_map<int, vastina::http*> clients;

public:
    server(size_t _maxevents = 50) :
        clients{ } {
        ep = new Epoll(_maxevents);
        //buffer = new Buffer(); 
        }
    ~server(){
        //close(serversock);
        for(auto &client:clients){ delete client.second; }
        delete ep;
    }
    void init();
    void setsock(int af,int type,int protocol, short port = 1453);
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
    if(listen(serversock, 20) == -1)
    {
        printf("listen fail, error code: %d\n", errno);
    }
    
}

void server::init(){
    ep->init(serversock);
}

void server::run(){
    struct sockaddr_in clnt_addr;
    socklen_t addr_sz = sizeof(clnt_addr) ;

    while (true)
    {
        int event_cnt = ep->Epoll_wait();
        if(event_cnt == -1)
        {

        }
        for(auto i=0; i<event_cnt; ++i)
        {
            if(ep->getfd(i)==serversock)
            {
                int clntsock = accept(serversock, (sockaddr*)&clnt_addr, &addr_sz);
                ep->epoll_add(clntsock);

                if(!clients[clntsock]) clients[clntsock] = new vastina::http();
                //std::cout <<"connceted: "<< clntsock <<'\n' ;
            }
            else
            {
                int str_len = read(ep->getfd(i), readbuffer, sizeof(readbuffer));

                if(str_len == 0)
                {
                    ep->epoll_del(i);
                }
                else
                {
                    int sock = ep->getfd(i);
                    clients[sock]->getreponse_test(readbuffer, sendbuffer);
                    write(sock, sendbuffer, sizeof(sendbuffer));
                }
            }
        }
    }
}

void server::end(){
    close(serversock);
}


#endif