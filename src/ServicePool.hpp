#include "Service.hpp"

#include <vector>

class ServicePool {
public:
    ServicePool(
        int, 
        const ThreadSafeDict<std::string, std::string>& dict);
    ~ServicePool();
    void add_service(int);
private:
    std::condition_variable signal_;
    std::mutex signal_mutex_;

    bool active_;

    std::condition_variable idle_signal_;
    std::mutex idle_signal_mutex_;

    std::atomic<int> socket_;
    std::atomic<int> count_;

    std::vector<Service> service_entities_;
};