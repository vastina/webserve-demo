//epoll stdin
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
    // 创建 epoll 实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return 1;
    }

    // 添加标准输入的文件描述符到 epoll 实例中
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        close(epoll_fd);
        return 1;
    }

    const int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];

    while (true) {
        // 使用 epoll_wait 等待事件发生
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < num_events; ++i) {
            std::cout << events[i].data.fd << " has data available\n";
            // 检查事件类型
            if (events[i].events & EPOLLIN) {
                // 标准输入上有可读事件
                std::string input;
                std::getline(std::cin, input);

                // 处理输入
                if (input.empty()) {
                    std::cout << "Exiting...\n";
                    break;
                } else {
                    std::cout << "Received input: " << input << "\n";
                }
            }
        }
    }

    close(epoll_fd);
    return 0;
}
