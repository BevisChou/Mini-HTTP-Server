#include "Pool.hpp"

Pool::Pool(int pool_size, const Mapping& mapping) :
        socket_(INVALID_SOCKET),
        active_(true),
        count_(pool_size)
{
    connections_.reserve(pool_size);
    for(int i = 0; i < pool_size; i++) {
        connections_.emplace_back(
            mapping, 
            socket_,
            signal_,
            signal_mutex_,
            active_,
            idle_signal_,
            idle_signal_mutex_,
            count_);
    }
}

Pool::~Pool()
{
    active_ = false;
    signal_.notify_all();
}

void Pool::add_connection(int socket)
{
    std::unique_lock<std::mutex> lk(idle_signal_mutex_);
    idle_signal_.wait(lk, [this]{return count_ > 0;});

    socket_ = socket;
    count_--;

    signal_.notify_one();
}