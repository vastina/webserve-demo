#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <iostream>

class Epoll
{
private:
    epoll_event *ep_events;
    epoll_event event;
    int epfd;
    size_t maxevents;
    const static int timeMs = -1;
public:
    Epoll(size_t _maxevents);
    ~Epoll();

    void init(int serversock);
    int Epoll_wait();
    void epoll_add(int fd);
    void epoll_del(int index);
    int getfd(int index);
};

Epoll::Epoll(size_t _maxevents = 50):
    epfd{epoll_create(_maxevents)},  
    maxevents{_maxevents} {
    ep_events = new epoll_event[_maxevents];
}

Epoll::~Epoll(){
    close(epfd);
    delete []ep_events;
}


void Epoll::init(int serversock){
    event.data.fd = serversock;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serversock, &event);
}

int Epoll::Epoll_wait(){
    return epoll_wait(epfd, ep_events, maxevents, timeMs);
}

void Epoll::epoll_add(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL,  flag | O_NONBLOCK);

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::epoll_del(int index){
    //std::cout << ep_events[index].data.fd <<" closed\n";
    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[index].data.fd, nullptr);
    close(ep_events[index].data.fd);
}

int Epoll::getfd(int index){
    return ep_events[index].data.fd;
}

#endif