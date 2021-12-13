#include "Service.hpp"

Service::Service(
    int id, 
    ThreadSafeQueue<int>& message_queue, 
    const ThreadSafeDict<std::string, 
    std::string>& dict) : 
        id_(id),
        active_(true),
        idle_(true),
        message_queue_(message_queue),
        dict_(dict),
        thread_(&Service::run, this) {}

Service::Service(Service&& other) :
        id_(other.id_),
        active_(other.active_),
        idle_(other.idle_),
        message_queue_(other.message_queue_),
        dict_(other.dict_),
        thread_(std::move(other.thread_)) {}

Service::~Service()
{
    active_ = false;
    if(thread_.joinable())
    {
        thread_.join();
    }
    close(socket_);
}

void Service::assign_work(int socket)
{
    socket_ = socket;
    idle_ = false;
}
    
void Service::run() const
{
    while(active_)
    {
        if(!idle_)
        {
            serve();
            close(socket_);
            idle_ = true;
            message_queue_.push(id_);
        }
    }
}

void Service::serve() const
{
}