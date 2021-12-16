#pragma once

#include "Payload.hpp"

#include <functional>
#include <mutex>
#include <fstream>
#include <sstream>

class Node;
class Mapping;

extern Mapping mapping_;

enum class ResourceType : uint8_t
{
    NONE,
    SERVICE,
    FILE
};

class Node;

class Mapping {
public:
    Mapping();
    ~Mapping();
    void add_service(
        std::string,
        Method,
        std::function<Response(const Request&)>);
    std::function<Response(const Request&)> get_service(
        std::string,
        Method) const;
    void add_file_path(std::string, Method, std::string);
    std::string get_file_path(std::string, Method) const;
    void remove(std::string, Method);
private:
    void pioneer(std::string);
    Node* get_endpoint(std::string) const;
    std::vector<std::string> get_route(std::string) const;
    std::mutex mutex_;
    Node* root_;
};

class Node {
public:
    Node(std::string);
    ~Node();
    void set_service(Method, std::function<Response(const Request&)>);
    void set_file(Method, std::string);
    void remove_resource(Method);
    const std::string& name() const;
    Node* get_child(const std::string&) const;
    Node* add_child(const std::string&);
    const std::function<Response(const Request&)>& get_service(Method) const;
    const std::string& get_file_path(Method) const;
private:
    std::string name_;
    std::vector<Node*> children_;
    std::vector<ResourceType> resource_types_;
    std::vector<std::pair<std::function<Response(const Request&)>, std::string>> resources_;
};

Response file_handler(const Request&);