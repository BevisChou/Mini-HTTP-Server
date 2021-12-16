#include "Pool.hpp"

#include <sys/poll.h>
#include <iostream>

class Server {
public:
    Server(int, int, Mapping&);
    ~Server();
    void run();
private:
    void serve();
    int accept_periodically();
    int socket_;
    bool active_;
    Mapping& mapping_;
    std::thread listener_;
    Pool pool_;
};
