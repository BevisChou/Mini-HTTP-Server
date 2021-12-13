#include "ThreadSafeDict.hpp"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "fcntl.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

const int INVALID_SOCKET = -1;

const int BUFFER_SIZE = 100000;

class Service {
public:
    Service(
        const ThreadSafeDict<std::string, std::string>&,
        std::atomic<int>&,
        std::condition_variable&,
        std::mutex&,
        bool&,
        std::condition_variable&,
        std::mutex&,
        std::atomic<int>&);
    Service(Service&&);
    ~Service();
private:
    void run();
    void serve() const;

    const std::atomic<int>& incoming_socket_;
    int socket_;
    const bool& active_;

    std::condition_variable& signal_;
    std::mutex& signal_mutex_;

    std::condition_variable& idle_signal_;
    std::mutex& idle_signal_mutex_;
    std::atomic<int>& count_;

    char buffer_[BUFFER_SIZE];
    const ThreadSafeDict<std::string, std::string>& dict_;
    std::thread thread_;
};