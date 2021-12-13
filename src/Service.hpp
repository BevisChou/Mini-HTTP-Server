#include "ThreadSafeQueue.hpp"
#include "ThreadSafeDict.hpp"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "fcntl.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

const int BUFFER_SIZE = 100000;

class Service {
public:
    Service(
        int, 
        ThreadSafeQueue<int>&, 
        const ThreadSafeDict<std::string, std::string>&);
    Service(Service&&);
    ~Service();
    void assign_work(int);
private:
    void run() const;
    void serve() const;
    int id_;
    int socket_;
    bool active_;

    mutable bool idle_;

    char buffer_[BUFFER_SIZE];
    ThreadSafeQueue<int>& message_queue_;
    const ThreadSafeDict<std::string, std::string>& dict_;
    std::thread thread_;
};