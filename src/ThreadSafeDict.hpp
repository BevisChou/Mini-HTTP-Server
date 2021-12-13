#include <unordered_map>
#include <mutex>

#include <iostream>

template<typename K, typename V>
class ThreadSafeDict {
public:
    ThreadSafeDict();
    ThreadSafeDict(const ThreadSafeDict&) = delete;
    ThreadSafeDict& operator=(const ThreadSafeDict&) = delete;
    ThreadSafeDict(ThreadSafeDict&&) = delete;
    ThreadSafeDict& operator=(ThreadSafeDict&&) = delete;
    void put(K, V);
    V get(K) const;
    void remove(K);
private:
    mutable std::mutex mutex_;
    std::unordered_map<K, V> dict_;
};

template<typename K, typename V>
ThreadSafeDict<K, V>::ThreadSafeDict() {}

template<typename K, typename V>
void ThreadSafeDict<K, V>::put(K key, V val)
{
    std::lock_guard<std::mutex> lock(mutex_);
    dict_[key] = val;
}
    
template<typename K, typename V>
V ThreadSafeDict<K, V>::get(K key) const
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto itor = dict_.find(key);
    if(itor == dict_.end())
    {
        throw -1;
    }
    return itor->second;
}

template<typename K, typename V>
void ThreadSafeDict<K, V>::remove(K key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    dict_.erase(key);
}