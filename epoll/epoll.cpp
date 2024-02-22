#include "epoll.hpp"

namespace vastina {

Epoll::Epoll(size_t _maxevents): 
	epfd{epoll_create(_maxevents)},
	maxevents{_maxevents}
{
	stdinchecker.epstdin = epoll_create(1);
	ep_events = new epoll_event[_maxevents];
}

Epoll::~Epoll() {
	close(epfd);
	delete[] ep_events;
}

bool Epoll::stdincheck(){
    if(epoll_wait(stdinchecker.epstdin, stdinchecker.event, 1, 1)){
        char input ;
        read(STDIN_FILENO, &input, 1);
std::cout << "input: " << input << '\n';
        if(input == 'q') return false;
    }   return true;
}

void Epoll::init(int serversock) {
	event.data.fd = STDIN_FILENO;
	event.events = EPOLLIN;
	epoll_ctl(stdinchecker.epstdin, EPOLL_CTL_ADD, STDOUT_FILENO, &event);

	event.data.fd = serversock;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serversock, &event);
}

int Epoll::Epoll_wait() {
	return epoll_wait(epfd, ep_events, maxevents, timeMs);
}

uint32_t Epoll::getevent(int index) { return ep_events[index].events; }

void Epoll::epoll_add(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | SOCK_NONBLOCK);

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::epoll_del(int fd) {
	event = {0};

	epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
}

int Epoll::getfd(int index) { return ep_events[index].data.fd; }


} // namespace vastina