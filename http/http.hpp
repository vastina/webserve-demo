#ifndef _HTTP_H_
#define _HTTP_H_

#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <set>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "../epoll/clock.hpp"
#include "httpparse.hpp"
#include "httpresponse.hpp"

namespace vastina{

class http{
private:
    int fd;
    httpparser parser;
    httpresponse *response;

    Clock *clock;
    int state;
    //bool stopflag;
    std::mutex lock;
    //std::condition_variable newMessage;

public:
    http():
        fd{-1}, 
        parser{ }, 
        response{new httpresponse()},
        //stopflag{true},
        state{NOT_WORKING} {
        clock = new Clock();};
    http(int _fd):
        fd{_fd}, 
        parser{}, 
        response{},
        //stopflag{true},
        state{NOT_WORKING} {
        clock = new Clock();};

    bool connection_check();
    void reset();
    void reponse_test(const char* readbuf, char *buf);

    enum STATE{
         NOT_WORKING = 0
        ,PROCESSING 
        ,NORMAL_END
        ,ERROR_END
    };
    void process();
    int getcallback();
};

void http::process(){
    char readbuf[BUFSIZ], sendbuf[BUFSIZ];
    int str_len;
    {
        std::unique_lock<std::mutex> lck(lock);
        state = STATE::PROCESSING;
    }
std::cout << "fd: "<<fd<<'\n';
    while(true){
        str_len = read(fd, readbuf, BUFSIZ);
std::cout << str_len <<" afswefafa\n";
        if(str_len == 0){
            {
                std::unique_lock<std::mutex> lck(lock);
                state = STATE::NORMAL_END;
            }
            return;
        }
        else if(str_len == -1){
            {
                std::unique_lock<std::mutex> lck(lock);
                state = STATE::ERROR_END;
std::cout <<"fd: "<<fd<<" error: " <<errno <<'\n';
                //todo: send error
            }
            return;
        }
        else{
            readbuf[str_len] = 0;
            reponse_test(readbuf, sendbuf);    
        }
    }
}

int http::getcallback(){
    return state;
}

void http::reset(){
    parser.reset();
    response->reset();
    state = NOT_WORKING;
}

void http::reponse_test(const char* readbuf, char *buf){
//std::cout << readbuf <<'\n';
    parser.autoparse(readbuf);
std::cout << parser.getMethod() <<'\n';
std::cout << parser.getPath() <<'\n';
std::cout << parser.getProtocol() <<'\n';
std::cout << parser.getBody() <<'\n';
    response->makereponse_test(parser, buf);
    write(fd, (void*)buf, BUFSIZ);
}

bool http::connection_check(){
    return clock->check_connection();
}

}

#endif // _HTTP_H_