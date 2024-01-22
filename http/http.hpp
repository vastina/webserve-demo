 #ifndef _HTTP_H_
#define _HTTP_H_

#include <fstream>
#include <iostream>
#include <regex>
#include <set>

#include "httpparse.hpp"

namespace vastina{

class http{
private:
    int state
    ;

public:
    http() : state{-1} {};
    ~http(){};

    friend void getreponse_test(const char* sendbuf, char *buf);
};

void getreponse_test(const char* readbuf, char *buf){
    std::regex regex_test("([^\r\n]+)");
    std::cmatch match1;
    if (std::regex_search(readbuf, match1, regex_test)) {
        //std::cout << "Content before first \\r\\n: " << match[0] << std::endl；
        std::string *re = new std::string{std::move(match1[1].str())};
        std::regex request_regex(R"((\w+) (\S+) (\S+))");
        std::smatch match2;
        if (std::regex_match(*re, match2, request_regex) ) {
            //std::cout << "Method: " << lmatch[1] << std::endl;
            //std::cout << "Path: " << lmatch[2] << std::endl;
            //std::cout << "Protocol: " << lmatch[3] << std::endl;
            if((match2[1]=="GET" && match2[2]=="/")){
                //memset(buf, 0, BUFSIZ);
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