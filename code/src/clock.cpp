#include "clock.hpp"

#include <iostream>

namespace vastina
{

    bool Clock::CheckConnection()
    {
        if (not_connected == false)
        {
            start = std::chrono::system_clock::now();
            not_connected = true;
            return false;
        }
        else if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() > TIMEOUT)
        {
            std::cout << "connection timeout" << '\n';
            return true;
        }
        return false;
    };

} // namespace vastina