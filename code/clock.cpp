#include "clock.hpp"
#include "config.hpp"

#include <iostream>

namespace vastina {

bool Clock::check_connection()
{
  if ( not not_connected ) {
    start = std::chrono::system_clock::now();
    not_connected = true;
    return false;
  } else if ( std::chrono::duration_cast<std::chrono::seconds>( std::chrono::system_clock::now() - start ).count()
              > config::connection_timeout ) {
    std::cout << "connection timeout" << '\n';
    return true;
  }
  return false;
};

void Clock::reset()
{
  start = std::chrono::system_clock::now();
  not_connected = false;
}

}