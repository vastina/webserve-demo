 #ifndef _HTTP_H_
#define _HTTP_H_

#include <fstream>

#include "httpparse.h"

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

void getreponse_test(const char* sendbuf, char *buf){
    std::ifstream infile;
    infile.open("response.txt", std::ifstream::in);
    while (!infile.eof())
    {
        infile.read(buf, BUFSIZE);
    }
    
}

}

#endif // _HTTP_H_