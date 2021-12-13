#include "Service.hpp"

Service::Service(
    const ThreadSafeDict<std::string, std::string>& dict,
    std::atomic<int>& incoming_socket,
    std::condition_variable& signal,
    std::mutex& signal_mutex,
    bool& active,
    std::condition_variable& idle_signal,
    std::mutex& idle_signal_mutex,
    std::atomic<int>& count) :
        active_(active),
        socket_(INVALID_SOCKET),
        dict_(dict),
        incoming_socket_(incoming_socket),
        signal_(signal),
        signal_mutex_(signal_mutex),
        idle_signal_(idle_signal),
        idle_signal_mutex_(idle_signal_mutex),
        count_(count),
        thread_(&Service::run, this) {}

Service::Service(Service&& other) :
        active_(other.active_),
        dict_(other.dict_),
        incoming_socket_(other.incoming_socket_),
        signal_(other.signal_),
        signal_mutex_(other.signal_mutex_),
        idle_signal_(idle_signal_),
        idle_signal_mutex_(other.idle_signal_mutex_),
        count_(other.count_),
        thread_(std::move(other.thread_)) {}

Service::~Service()
{
    if(thread_.joinable())
    {
        thread_.join();
    }
    close(socket_);
}
    
void Service::run()
{
    while(active_)
    {
        std::unique_lock<std::mutex> signal_lk(signal_mutex_);
        signal_.wait(signal_lk);

        if(active_)
        {
            socket_ = incoming_socket_;
            // signal_lk.unlock();

            serve();
            count_++;
            
            idle_signal_.notify_one();
            close(socket_);
        }
    }
}

void Service::serve() const
{
}