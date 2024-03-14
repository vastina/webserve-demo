#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/unistd.h>

namespace vastina
{

    class Epoll
    {

      private:
        epoll_event* ep_events;
        epoll_event event;
        int epfd;
        size_t maxevents;
        const static int timeMs = -1;

        struct _stdincheck
        {
            epoll_event event[1];
            int epstdin;
        } stdinchecker;

      public:
        Epoll(size_t _maxevents = 50);
        ~Epoll();

        void Init(int serversock);
        int EpollWait();
        uint32_t getEvent(int index);
        void EpollAdd(int fd);
        void EpollDelet(int index);
        int getFD(int index);

        bool stdincheck();
    };

} // namespace vastina

#endif