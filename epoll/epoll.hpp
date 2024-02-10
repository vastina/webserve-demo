#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <fcntl.h>
#include <iostream>
#include <queue>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/unistd.h>

namespace vastina {

class Epoll {
  private:
	epoll_event *ep_events;
	epoll_event event;
	int epfd;
	size_t maxevents;
	const static int timeMs = -1;
	// struct _stdincheck{
	//     epoll_event event;
	//     int epstdin;
	// } stdinchecker;
  public:
	Epoll(size_t _maxevents);
	~Epoll();

	void init(int serversock);
	int Epoll_wait();
	uint32_t getevent(int index);
	void epoll_add(int fd);
	void epoll_del(int index);
	int getfd(int index);

	// bool stdincheck();
};

Epoll::Epoll(size_t _maxevents = 50)
	: epfd{epoll_create(_maxevents)},
	  maxevents{
		  _maxevents} /*,
 stdinchecker{epoll_event{EPOLLIN|EPOLLET, STDIN_FILENO},  epoll_create(1)}*/
{
	ep_events = new epoll_event[_maxevents];
}

Epoll::~Epoll() {
	close(epfd);
	delete[] ep_events;
}

// bool Epoll::stdincheck(){
//     if(epoll_wait(stdinchecker.epstdin, &stdinchecker.event, 1, timeMs)){
//         std::string input{" "};
//         std::getline(std::cin, input);
//         if(input[0] == 'q') return false;
//     }   return true;
// }

void Epoll::init(int serversock) {
	// epoll_ctl(stdinchecker.epstdin, EPOLL_CTL_ADD, STDIN_FILENO,
	// &stdinchecker.event);

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
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);

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

#endif