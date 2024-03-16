#include "serve.hpp"

void server::setSocket(int af, int type, int protocol, short port)
{
    serversock = socket(af, type, protocol);
    int reuse = 1, nodelay = 1;
    setsockopt(serversock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    setsockopt(serversock, IPPROTO_TCP, 1, &nodelay, sizeof(nodelay));
    int flag = fcntl(serversock, F_GETFL, 0);
    fcntl(serversock, F_SETFL, flag | O_NONBLOCK);

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = af;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    if (bind(serversock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
    {
        printf("bind fail, error code: %d\n", errno);
    }
    if (::listen(serversock, 100) == -1)
    {
        printf("listen fail, error code: %d\n", errno);
    }
}

void server::Init()
{
    ep->Init(serversock);
    stopflag = true;
    vastina::CacheTree::getInstance().InitRead();
}

void server::CloseFD(int fd)
{
    ep->EpollDelet(fd);
    close(fd);
    // delete clients[fd];
    clients.erase(fd);
}

void server::Run()
{

    std::cout << "Enter q to quit\n";
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    while (stopflag)
    {
        int event_cnt = ep->EpollWait();
        if (event_cnt == -1)
        { // until now, I haven't met this situation
            printf("EpollWait fail, error code: %d\n", errno);
        }
        for (auto i = 0; i < event_cnt; ++i)
        {
            if (ep->getEvent(i) & EPOLLIN)
            {
                if (ep->getFD(i) == serversock)
                {
                    int clntsock = accept(serversock, (struct sockaddr*)&addr, &len);
                    ep->EpollAdd(clntsock);

                    if (!clients[clntsock])
                        clients[clntsock] = std::make_unique<vastina::Http>(clntsock);
                    else
                        clients[clntsock]->Reset();

                    std::cout << "fd: " << clntsock << '\n';

                    pool.Enqueue([this, clntsock]() { clients[clntsock]->Process(); });
                }
                else
                {
                    int clntsock = ep->getFD(i);
                    int callback = clients[clntsock]->getcallback();
                    std::cout << "fd: " << clntsock << " callback: " << callback << '\n';
                    if (callback == vastina::Http::STATE::NORMAL_End)
                    {
                        if (!clients[clntsock]->ConnectionCheck())
                            CloseFD(clntsock);
                    }
                    else if (callback == vastina::Http::STATE::ERROR_End)
                    {
                        if (errno != EAGAIN && errno != EWOULDBLOCK)
                            CloseFD(clntsock);
                        // todo:log it
                    }
                }
            }
            if (ep->getEvent(i) & EPOLLHUP)
            {
                int sock = ep->getFD(i);
                clients[sock].~unique_ptr();
            }
        }
        stopflag = ep->stdincheck();
    }
}

void server::End()
{
    close(serversock);
    clients.clear();
    ep->~Epoll();
}