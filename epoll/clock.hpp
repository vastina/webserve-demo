#ifndef _COLOCK_H_
#define _COLOCK_H_

#include <chrono>
#include <iostream>

namespace vastina{

constexpr int TIMEOUT = 10;

struct Clock
{
    std::chrono::time_point<std::chrono::system_clock> start;
    bool not_connected;
    bool check_connection();
};

bool Clock::check_connection(){
    if(not_connected == false){
        start = std::chrono::system_clock::now();
        not_connected = true;
        return false;
    } 
    else if(std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now() - start).count() > TIMEOUT){
            std::cout << "connection timeout" << '\n';
            return true;
    }
    return false;
}

}

#endif