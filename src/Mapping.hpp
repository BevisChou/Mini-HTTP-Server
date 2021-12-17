#pragma once

#include "Payload.hpp"
#include "Util.hpp"

#include <functional>
#include <iostream>

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
    void remove(std::string, Method);
    void show();
private:
    void pioneer(std::string);
    Node* get_endpoint(std::string) const;
    std::vector<std::string> get_route(std::string) const;
    Node* root_;
};

class Node {
public:
    Node(std::string);
    ~Node();
    void set_service(Method, std::function<Response(const Request&)>);
    void remove_resource(Method);
    const std::string& name() const;
    Node* get_child(const std::string&) const;
    Node* add_child(const std::string&);
    const std::function<Response(const Request&)>& get_service(Method) const;
    void show(std::string);
private:
    std::string name_;
    std::vector<Node*> children_;
    std::vector<bool> status_;
    std::vector<std::function<Response(const Request&)>> services_;
};