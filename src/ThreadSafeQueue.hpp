#include <queue>
#include <mutex>

#include <iostream>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue();
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue(ThreadSafeQueue&&) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;
    void push(T);
    void pop(T&);
    bool empty() const;
private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
};

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue() {}

template<typename T>
void ThreadSafeQueue<T>::push(T val)
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(val);
}
    
template<typename T>
void ThreadSafeQueue<T>::pop(T& value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(queue_.empty())
    {
        throw -1;
    }
    value = queue_.front();
    queue_.pop();
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}