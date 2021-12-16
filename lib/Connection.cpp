#include "Connection.hpp"

Connection::Connection(
    const Mapping& mapping,
    std::atomic<int>& incoming_socket,
    std::condition_variable& signal,
    std::mutex& signal_mutex,
    bool& active,
    std::condition_variable& idle_signal,
    std::mutex& idle_signal_mutex,
    std::atomic<int>& count) :
        active_(active),
        socket_(INVALID_SOCKET),
        mapping_(mapping),
        incoming_socket_(incoming_socket),
        signal_(signal),
        signal_mutex_(signal_mutex),
        idle_signal_(idle_signal),
        idle_signal_mutex_(idle_signal_mutex),
        count_(count),
        thread_(&Connection::run, this) {}

Connection::Connection(Connection&& other) :
        active_(other.active_),
        mapping_(other.mapping_),
        incoming_socket_(other.incoming_socket_),
        signal_(other.signal_),
        signal_mutex_(other.signal_mutex_),
        idle_signal_(idle_signal_),
        idle_signal_mutex_(other.idle_signal_mutex_),
        count_(other.count_),
        thread_(std::move(other.thread_)) {}

Connection::~Connection()
{
    if(thread_.joinable())
    {
        thread_.join();
    }
    close(socket_);
}
    
void Connection::run()
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

void Connection::serve() const
{
    std::memset((void *)buffer_, 0 , sizeof(buffer_));
    read(socket_ , (void *)buffer_, BUFFER_SIZE);

    std::string request_str(buffer_);
    Request request = Interpreter::string_to_request(request_str);

    auto functor = mapping_.get_service(request.url(), request.method());
    std::string response_str = Interpreter::response_to_string(functor(request));

    std::memset((void *)buffer_, 0 , sizeof(buffer_));
    std::memcpy((void *)buffer_, response_str.c_str(), response_str.size());
    write(socket_ , buffer_, response_str.size());
}