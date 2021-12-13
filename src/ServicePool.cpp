#include "ServicePool.hpp"

ServicePool::ServicePool(
    int pool_size, 
    const ThreadSafeDict<std::string, 
    std::string>& dict) 
{
    service_entities_.reserve(pool_size);
    for(int i = 0; i < pool_size; i++)
    {
        free_services_.push(i);
        service_entities_.emplace_back(i, free_services_, dict);
    }
}

ServicePool::~ServicePool() {}

void ServicePool::add_service(int socket)
{
    while(free_services_.empty());
    int id;
    free_services_.pop(id);
    
    if(fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) & ~O_NONBLOCK) < 0)
    {
        throw -1;
    }
    
    service_entities_[id].assign_work(socket);
}