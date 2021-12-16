#include "Connection.hpp"

#include <vector>

class Pool {
public:
    Pool(int, const Mapping&);
    ~Pool();
    void add_connection(int);
private:
    std::condition_variable signal_;
    std::mutex signal_mutex_;

    bool active_;

    std::condition_variable idle_signal_;
    std::mutex idle_signal_mutex_;

    std::atomic<int> socket_;
    std::atomic<int> count_;

    std::vector<Connection> connections_;
};