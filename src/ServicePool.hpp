#include "Service.hpp"

#include <vector>

class ServicePool {
public:
    ServicePool(
        int, 
        const ThreadSafeDict<std::string, 
        std::string>& dict);
    ~ServicePool();
    void add_service(int);
private:
    ThreadSafeQueue<int> free_services_;
    std::vector<Service> service_entities_;
};