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

#include "log/loger.hpp"
#include "http/http.hpp"
#include "buffer/buffer.hpp"
#include "epoll/epoll.hpp"
#include "pool/ThreadPool.hpp"

//加了namespace vastina之后，所有线程大部分时间都会一直卡在epoll_wait
//往threadpool里面加入新任务之后也不起任何作用

class server{
private:
    int serversock;

    vastina::Epoll *ep;

    //char readbuffer[BUFSIZ], sendbuffer[BUFSIZ] ; 
    //Buffer *buffer;
    //to be finished as a class or struct
    bool stopflag;

    std::unordered_map<int, vastina::http*> clients;
    
    ThreadPool pool;
public:
    server(size_t _maxevents = 50) :
        clients{ },
        pool{ThreadPool(4)} {
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
                    if(!clients[clntsock]) clients[clntsock] = new vastina::http(clntsock);
                    //else clients[clntsock]->reset(); todo
                    std::cout << "fd: "<< clntsock << '\n';
                    pool.enqueue([this, clntsock](){
                        clients[clntsock]->process();
                    });
                } 
                else
                {
                    int clntsock = ep->getfd(i);
                    int callback = clients[clntsock]->getcallback();
std::cout << "fd: "<< clntsock << " callback: " << callback << '\n';
                    if(callback == vastina::http::STATE::NORMAL_END)
                    {
                        closeFD(clntsock);
                    }
                    else if(callback == vastina::http::STATE::ERROR_END)
                    {
                        closeFD(clntsock);
                        //todo:log it
                    }
                    // int str_len = read(ep->getfd(i), readbuffer, sizeof(readbuffer));
                    // if(str_len == 0)
                    // {
                    //     static int count = 0;
                    //     printf("count: %d, fd: %d\n", ++count, ep->getfd(i));

                    //     int sock = ep->getfd(i);
                    //     //if(clients[sock]->connection_check())
                    //         closeFD(sock);
                    // }
                    // else
                    // {
                    //     int sock = ep->getfd(i);

                    //     auto t1 = std::chrono::system_clock::now();

                    //     clients[sock]->getreponse_test(readbuffer, sendbuffer);
                    //     write(sock, sendbuffer, sizeof(sendbuffer));

                    //     auto t2 = std::chrono::system_clock::now();
                    //     std::cout << "cost time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds\n";
                    // }
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
