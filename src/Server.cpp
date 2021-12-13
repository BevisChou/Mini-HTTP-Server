#include "Server.hpp"

const int MAX_QUEUE_SIZE = 100;

Server::Server(int port, int service_pool_size) : 
    service_pool_(service_pool_size, dict_), 
    active_(true) 
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
    std::string in1, in2;
    while(true)
    {
        std::cin >> opt;
        switch (opt)
        {
            case 'p':
                std::cin >> in1 >> in2;
                dict_.put(in1, in2);
                break;
            case 'g':
                std::cin >> in1;
                std::cout << dict_.get(in1) << std::endl;
                break;
            case 'r':
                std::cin >> in1;
                dict_.remove(in1);
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
        // Low CPU usage guaranteed.
        if((data_sock = accept_periodically()) >= 0)
        {
            service_pool_.add_service(data_sock);
        }
    }
}

int Server::accept_periodically()
{
    int ret = accept(socket_, NULL, NULL);

    if(ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return ret;
}