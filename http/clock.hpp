#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <chrono>
#include <iostream>

namespace vastina{

struct Clock
{
    bool not_connected = false;
    std::chrono::system_clock::time_point start;
    //std::chrono::system_clock::time_point end;
};

}


#endif//_CLOCK_H_