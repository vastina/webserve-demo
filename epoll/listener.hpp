#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <iostream>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <unordered_map>

#include "epoll.hpp"
#include "../http/http.hpp"
#include "../log/loger.hpp"

namespace vastina{

class Listener{
private:
    int serversock;
    Epoll *ep;
    std::unordered_map<int, vastina::http*> clients;

public:
    Listener();
    void setserversock(int af,int type,int protocol, short port);
    void accepter();
    void closeFd(int fd);
};

Listener::Listener(){
    ep = new Epoll();
    ep->init(serversock);
}

void Listener::closeFd(int fd){
    ep->epoll_del(fd);
    delete clients[fd];
    clients.erase(fd);
}

void Listener::setserversock(int af,int type,int protocol, short port){
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

void Listener::accepter(){
    bool flag = true;

    while (true)
    {
        int event_cnt = ep->Epoll_wait();
        if(event_cnt == -1)
        {

        }
        for(auto i=0; i<event_cnt; ++i)
        {
            if(ep->stdincheck(i)) flag = false;

            if(ep->getevent(i) & EPOLLIN) //ep->getfd(i) == serversock
            {
                if(ep->getfd(i) == serversock)
                {
                    int clntsock = accept(serversock, nullptr, nullptr);
                    ep->epoll_add(clntsock);
                    if(!clients[clntsock]) clients[clntsock] = new vastina::http();
                } 
                else
                {
                    // int str_len = read(ep->getfd(i), readbuffer, sizeof(readbuffer));
                    // if(str_len == 0)
                    // {
                    //     std::cout << "closed: " << ep->getfd(i) << '\n';
                    //     ep->epoll_del(i);
                    // }
                    // else
                    // {
                    //     int sock = ep->getfd(i);
                    //     clients[sock]->getreponse_test(readbuffer, sendbuffer);
                    //     write(sock, sendbuffer, sizeof(sendbuffer));
                    // }
                }
            }
            if(ep->getevent(i) & EPOLLHUP)
            {
                int sock = ep->getfd(i);
                delete clients[sock];
            }

        }
    }
}

}



#endif // _LISTENER_H_