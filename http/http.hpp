#ifndef _HTTP_H_
#define _HTTP_H_

#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <cstring>

#include "clock.hpp"
#include "httpparse.hpp"
#include "httpresponse.hpp"

namespace vastina{

class http{
private:
    int fd;
    Clock clock;
    httpparser parser;
    httpresponse response;
public:
    http():fd{-1}, parser{}, response{} {};
    http(int _fd): fd{_fd}, parser{}, response{} {};

    bool connection_check();
    void getreponse_test(const char* readbuf, char *buf);

};

void http::getreponse_test(const char* readbuf, char *buf){
    parser.autoparse(readbuf);
    response.makereponse_test(parser, buf);
    //no buffer now, so send parser directly
}

bool http::connection_check(){
    if(clock.not_connected == false){
        clock.start = std::chrono::system_clock::now();
        clock.not_connected = true;
        return false;
    } 
    else if(std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now() - clock.start).count() > 20)
        return true;
    return false;
}

}

#endif // _HTTP_H_