#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/unistd.h>

class Buffer {
  public:
	Buffer() : read_cnt{0}, send_cnt{0}, last_read{0}, last_send{0} {
		memset(readbuffer, 0, sizeof(readbuffer));
		memset(sendbuffer, 0, sizeof(sendbuffer));
		fs = new std::fstream();
	};
	~Buffer() {
		// delete []readbuffer;    delete []sendbuffer;
		delete fs;
	}

  private:
	char readbuffer[BUFSIZ], sendbuffer[BUFSIZ];
	size_t read_cnt, send_cnt;
	size_t last_read, last_send;
	std::fstream *fs;
	// std::mutex bmutex;
  public:
	void resetread();
	void resetsend();

	size_t get_readcount();
	size_t get_sendcount();

	void readn(size_t n, int fd);
	int autoread(int fd);
	void sendn(size_t n, int fd);
	void autosend(int fd, const char *filename);

	const char *readbuffer_ptr();
	const char *buffer_lastread();
	const char *sendbuffer_ptr();
};

void Buffer::resetread() {
	// std::lock_guard<std::mutex> lg(bmutex);
	read_cnt = 0;
	last_read = 0;
	bzero(readbuffer, BUFSIZ);
}

void Buffer::resetsend() {
	send_cnt = 0;
	last_send = 0;
	bzero(sendbuffer, BUFSIZ);
}

size_t Buffer::get_readcount() { return read_cnt - last_read; }
size_t Buffer::get_sendcount() { return send_cnt - last_send; }

void Buffer::readn(size_t n, int fd) {
	if (read_cnt + n > BUFSIZ)
		resetread();
	last_read = read_cnt;
	if (read(fd, (void *)(readbuffer + read_cnt), n) == -1)
		printf("buffer.hpp void Buffer::readn(size_t n, int fd), %d", errno);
	readbuffer[(read_cnt += n)] = 0;
}

int Buffer::autoread(int fd) {
	int count = -1;
	if ((count = read(fd, (void *)(readbuffer + read_cnt),
					  BUFSIZ - read_cnt)) == -1)
		std::cout << __FILE__ << " Buffer::autoread\n";
	else if (read_cnt + count > 8000)
		resetread();
	else {
		last_read = read_cnt;
		read_cnt += count;
	}
	printf("count:%d readcnt:%ld lastread:%ld\n", count, read_cnt, last_read);
	return count;
}

void Buffer::sendn(size_t n, int fd) {
	if (send_cnt + n > BUFSIZ)
		resetsend();
	last_send = send_cnt;
	;
	if (write(fd, (void *)(sendbuffer + send_cnt), n) == -1)
		printf("buffer.hpp void Buffer::writen(size_t n, int fd), %d", errno);
	sendbuffer[(send_cnt += n)] = 0;
}

void Buffer::autosend(int fd, const char *filename) {
	fs->open(filename, std::ifstream::in);
	if (!fs->is_open()) {
		printf("buffer.hpp void Buffer::autosend(int fd, const char* "
			   "filename), %d",
			   errno);
		return;
	}
	int count = -1;
	while (!fs->eof()) {
		fs->read(sendbuffer, BUFSIZ >> 5);
		if ((count = write(fd, sendbuffer, BUFSIZ >> 5)) == -1)
			std::cout << __FILE__ << "autosendn";
		if (send_cnt + count > BUFSIZ - 100)
			resetsend();
		else {
			last_send = send_cnt;
			send_cnt += count;
		}
	}
	fs->close();
}

const char *Buffer::readbuffer_ptr() {
	// static char *ptr;
	// ptr = (readbuffer + last_read);
	// std::cout << *(readbuffer + last_read) <<"ptr\n";
	return (readbuffer + read_cnt);
}

const char *Buffer::buffer_lastread() { return (readbuffer + last_read); }

const char *Buffer::sendbuffer_ptr() {
	static char *ptr;
	ptr = (readbuffer + last_send);
	return ptr;
}

#endif