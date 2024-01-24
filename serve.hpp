#ifndef _SERVE_SOCK_H_
#define _SERVE_SOCK_H_


#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>  
#include <unordered_map>

#include "tools.h"
#include "log/loger.hpp"
#include "http/http.hpp"
#include "buffer/buffer.hpp"

class server{
private:
    int serversock;
    epoll_event *ep_events;
    epoll_event event;
    int epfd, event_cnt;
    size_t maxevents;

    //char readbuffer[BUFSIZE], sendbuffer[BUFSIZE] ; //to be finished as a class or struct
    Buffer *buffer;

    std::unordered_map<int, vastina::http*> clients;

public:
    server(size_t _maxevents = 200) :
        epfd{epoll_create(_maxevents)},  
        maxevents{_maxevents}, 
        clients{ } {
        ep_events = new epoll_event[_maxevents];
        buffer = new Buffer(); }
    ~server(){
        delete ep_events;
        for(auto &client:clients){ delete client.second; }
    }
    void init();
    void setsock(int af,int type,int protocol, short port = 1453);
    void run();
    void end();
};

void server::setsock(int af,int type,int protocol, short port){
    serversock = socket(af, type, protocol);
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = af;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //serveraddr.sin_addr.s_addr = inet_addr("47.107.39.53");
    serveraddr.sin_port = htons(port) ;
    if(bind(serversock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        printf("bind fail, error code: %d\n", errno);
        //errorhandling("bind fail, error code: {}\n", errno);
    }
    if(listen(serversock, 6) == -1)
    {
        printf("listen fail, error code: %d\n", errno);
        //vastina::logtest(std::format("listen fail, error code: {}\n", errno).c_str());
    }
    
}

void server::init(){
    event.data.fd = serversock ;
    event.events = EPOLLIN ;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serversock, &event);
    // memset(readbuffer, 0, sizeof(readbuffer));
    // memset(sendbuffer, 0, sizeof(sendbuffer));
}

void server::run(){
    struct sockaddr_in clnt_addr;
    socklen_t addr_sz = sizeof(clnt_addr) ;

    while (true)
    {
        event_cnt = epoll_wait(epfd, ep_events, maxevents, -1);
        if(event_cnt == -1)
        {

        }
        for(auto i=0; i<event_cnt; ++i)
        {
            if(ep_events->data.fd==serversock)
            {
                int clntsock = accept(serversock, (sockaddr*)&clnt_addr, &addr_sz);
                event.events = EPOLLIN ;
                event.data.fd = clntsock ;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clntsock, &event);
                clients[clntsock] = new vastina::http();
                std::cout <<"connceted: "<< clntsock <<'\n' ;
            }
            else
            {
                buffer->autoread(ep_events->data.fd);

                if(buffer->get_readcount() == 0)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, nullptr);
                    close(ep_events[i].data.fd);
                    printf("%d closed\n", ep_events[i].data.fd);
                }
                else
                {
                    char path[100] ;    bzero(path, sizeof(path));
                    clients[ep_events[i].data.fd]->getreponse_test(buffer->readbuffer_ptr(), path);
                    buffer->autosend(ep_events[i].data.fd, path);
                }
            }
        }
    }
    
}

void server::end(){
    close(serversock);
    close(epfd);
}


#endif