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
//应该与ThreadPool不在namespace里面有关

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

#endif
