#include "http.hpp"

#include <iostream>

namespace vastina
{

    Http::Http() : fd{-1}, parser{}, response{}, state{NOT_WORKING}, clock{new Clock()} {};

    Http::Http(int _fd) : fd{_fd}, parser{}, response{}, state{NOT_WORKING}, clock{new Clock()} {};

    void Http::Process()
    {
        thread_local char readbuf[BUFSIZ], sEndbuf[BUFSIZ];
        thread_local int str_len;
        {
            std::unique_lock<std::mutex> lck(lock);
            state = STATE::PROCESSING;
        }
        std::cout << "fd: " << fd << '\n';
        while (true)
        {
            str_len = read(fd, readbuf, BUFSIZ);
            if (str_len == 0)
            {
                {
                    std::unique_lock<std::mutex> lck(lock);
                    state = STATE::NORMAL_End;
                }
                return;
            }
            else if (str_len == -1)
            {
                {
                    std::unique_lock<std::mutex> lck(lock);
                    state = STATE::ERROR_End;
                    std::cout << "fd: " << fd << " error: " << errno << '\n';
                    // todo: sEnd error
                }
                return;
            }
            else
            {
                readbuf[str_len] = 0;
                reponse_test(readbuf, sEndbuf);
            }
        }
    }

    int Http::getcallback()
    {
        std::unique_lock<std::mutex> lck(lock);
        return state;
    }

    void Http::Reset()
    {
        parser.Reset();
        response.Reset();
        state = NOT_WORKING;
    }

    void Http::reponse_test(const char* readbuf, char* buf)
    {

        auto t1 = std::chrono::high_resolution_clock::now();
        // std::cout << readbuf <<'\n';

        parser.autoparse(readbuf);

        std::cout << parser.getMethod() << '\n';
        std::cout << parser.getPath() << '\n';
        std::cout << parser.getProtocol() << '\n';
        std::cout << parser.getBody() << '\n';

        response.autoResponse(parser, buf);
        write(fd, (void*)buf, BUFSIZ);

        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "response time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << '\n';
    }

    bool Http::ConnectionCheck() { return clock->CheckConnection(); }

} // namespace vastina
