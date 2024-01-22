
#include "buffer.hpp"

void Buffer::resetread(){
    //std::lock_guard<std::mutex> lg(bmutex);
        read_cnt = 0;
        memset(readbuffer, 0, BUFSIZ);
}

