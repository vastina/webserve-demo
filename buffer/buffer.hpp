#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <iostream>
#include <format>
#include <cstring>
#include <string.h>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <sys/unistd.h>

struct Buffer {
    Buffer() : read_cnt{0}, send_cnt{0} {
        memset(readbuffer, 0, sizeof(readbuffer));
        memset(sendbuffer, 0, sizeof(sendbuffer));
    }

    char readbuffer[BUFSIZ], sendbuffer[BUFSIZ];
    int read_cnt, send_cnt;
    std::mutex bmutex;

    void resetread();
    void resetsend();

    void readn();
};


#endif