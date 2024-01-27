#ifndef _HTTP_H_
#define _HTTP_H_

#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <cstring>

#include "httpparse.hpp"

namespace vastina{

class http{
private:
    int state
    ;

public:
    http() : state{-1} {};
    ~http(){};

    void reset();
    void getreponse_test(const char* sendbuf, char *buf);
};

void http::reset(){

}

void http::getreponse_test(const char* readbuf, char *buf){
    //std::cout << readbuf <<'\n';
    std::regex regex_test("([^\r\n]+)");
    std::cmatch match1;
    if (std::regex_search(readbuf, match1, regex_test)) {
        std::string *re = new std::string{std::move(match1[1].str())};
        std::regex request_regex(R"((\w+) (\S+) (\S+))");
        std::smatch match2;
        if (std::regex_match(*re, match2, request_regex) ) {
            if((match2[1]=="GET" && match2[2]=="/")){
                //if(match2[1]=="GET" && match2[2]=="/")
                //  strcpy(path, "http/response.txt");
                //lse printf("fuck you\n");
                std::ifstream infile;
                infile.open("http/response.txt", std::ifstream::in);
                while (!infile.eof())
                {
                    infile.read(buf, BUFSIZ);
                }
                infile.close();
            }
        }
        else{printf("fuck you\n");}
        delete re;
    } 
    else{printf("fuck you too\n");}

}

}

#endif // _HTTP_H_