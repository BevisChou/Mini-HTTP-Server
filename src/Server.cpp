#include "Server.hpp"

const int MAX_QUEUE_SIZE = 100;

Server::Server(int port, int pool_size, Mapping& mapping) : 
        active_(true),
        mapping_(mapping),
        pool_(pool_size, mapping_)
{
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        throw -1;
    }

    if(fcntl(socket_, F_SETFL, fcntl(socket_, F_GETFL, 0) | O_NONBLOCK) < 0)
    {
        throw -1;
    }

    sockaddr_in addr;
    std::memset((char *)&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(socket_, (sockaddr *)&addr, sizeof(addr)) < 0) 
    { 
        throw -1;
    }

    if(listen(socket_, MAX_QUEUE_SIZE) == -1)
    {
        throw -1;
    }

    listener_ = std::move(std::thread(&Server::serve, this));
}

Server::~Server()
{
    listener_.join();
    close(socket_);
}

void Server::run()
{
    char opt;
    while(true)
    {
        std::cin >> opt;
        switch (opt)
        {
            case 'i':
                mapping_.show();
                break;
            case 'q':
                active_ = false;
                return;
            default:
                break;
        }
    }
}

void Server::serve()
{
    int data_sock;

    while(active_)
    {
        if((data_sock = accept_periodically()) >= 0)
        {
            pool_.add_connection(data_sock);
        }
    }
}

int Server::accept_periodically()
{
    int ret;

    pollfd fds[1];
    memset(fds, 0 , sizeof(fds));
    fds[0].fd = socket_;
    fds[0].events = POLLIN;
    
    int rc = poll(fds, 1, 100);

    if (rc < 0)
    {
        throw -1;
    }
    else if (rc == 0)
    {
        ret = -1;
    }
    else
    {
        ret = accept(socket_, NULL, NULL);
        if (ret < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK))
        {
            throw -1;
        }
    }

    return ret;
}