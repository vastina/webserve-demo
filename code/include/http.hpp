#ifndef _HTTP_H_
#define _HTTP_H_

#include <arpa/inet.h>
#include <mutex>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "clock.hpp"
#include "httpparse.hpp"
#include "httpresponse.hpp"

namespace vastina
{

    class Http
    {
      private:
        int fd;
        HttpParser parser;
        HttpResponse response;

        int state;
        Clock* clock;

        std::mutex lock;

      public:
        Http();
        Http(int _fd);

        bool ConnectionCheck();
        void Reset();
        void reponse_test(const char* readbuf, char* buf);

        enum STATE
        {
            NOT_WORKING = 0,
            PROCESSING,
            NORMAL_End,
            ERROR_End
        };
        void Process();
        int getcallback();
    };

} // namespace vastina

#endif // _HTTP_H_