#include "ServicePool.hpp"

#include <sys/poll.h>
#include <sys/time.h>

class Server {
public:
    Server(int, int);
    ~Server();
    void run();
private:
    void serve();
    int socket_;
    bool active_;
    ThreadSafeDict<std::string, std::string> dict_;
    std::thread listener_;
    ServicePool service_pool_;
};
