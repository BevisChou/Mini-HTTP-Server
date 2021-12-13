#include "ServicePool.hpp"

ServicePool::ServicePool(
    int pool_size, 
    const ThreadSafeDict<std::string, std::string>& dict) :
        socket_(INVALID_SOCKET),
        active_(true),
        count_(pool_size)
{
    service_entities_.reserve(pool_size);
    for(int i = 0; i < pool_size; i++) {
        service_entities_.emplace_back(
            dict, 
            socket_,
            signal_,
            signal_mutex_,
            active_,
            idle_signal_,
            idle_signal_mutex_,
            count_);
    }
}

ServicePool::~ServicePool()
{
    active_ = false;
    signal_.notify_all();
}

void ServicePool::add_service(int socket)
{
    std::unique_lock<std::mutex> lk(idle_signal_mutex_);

    idle_signal_.wait(lk, [this]{return count_ > 0;});
    
    if(fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) & ~O_NONBLOCK) < 0)
    {
        throw -1;
    }

    socket_ = socket;
    count_--;

    lk.unlock();
    signal_.notify_one();
}