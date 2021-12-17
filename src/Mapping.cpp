#include "Mapping.hpp"

// Mapping

Mapping::Mapping() : root_(new Node("")) {}

Mapping::~Mapping()
{
    delete root_;
}

void Mapping::add_service(
    std::string url, 
    Method method,
    std::function<Response(const Request&)> service)
{
    pioneer(url);
    get_endpoint(url)->set_service(method, service);
}

std::function<Response(const Request&)> Mapping::get_service(
    std::string url, 
    Method method) const
{
    Node* endpoint = get_endpoint(url);
    if(!endpoint) throw -1;
    return endpoint->get_service(method);
}

void Mapping::remove(std::string url, Method method)
{
    get_endpoint(url)->remove_resource(method);
}

void Mapping::show()
{
    std::cout << "MAPPING STATUS" << std::endl << "--------------" << std::endl;
    root_->show("");
    std::cout << "--------------" << std::endl;
}

void Mapping::pioneer(std::string url)
{
    Node* pioneer = root_;
    Node* last;
    std::vector<std::string> route = get_route(url);

    for(auto& point : route)
    {
        last = pioneer;
        pioneer = pioneer->get_child(point);
        if(!pioneer)
        {
            pioneer = last->add_child(point);
        }
    }
}

Node* Mapping::get_endpoint(std::string url) const
{
    std::vector<std::string> route = get_route(url);

    Node* ret = root_;
    for(auto& point : route)
    {
        ret = ret->get_child(point);
    }
    return ret;
}

std::vector<std::string> Mapping::get_route(std::string url) const
{
    // The string has been processed to be in the form of [/...][/...]...
    url.erase(0, 1);
    return Util::split(url, '/');
}

// Node

Node::Node(std::string name) :
    name_(name),
    status_(ENUM_SIZE_METHOD, false),
    services_(ENUM_SIZE_METHOD) {}

Node::~Node()
{
    for(Node* child : children_)
    {
        delete child;
    }
}

void Node::set_service(Method method, std::function<Response(const Request&)> service)
{
    int pos = static_cast<std::underlying_type<Method>::type>(method);
    status_[pos] = true;
    services_[pos] = service;
}

void Node::remove_resource(Method method)
{ 
    int pos = static_cast<std::underlying_type<Method>::type>(method);
    status_[pos] = false;
}

const std::string& Node::name() const
{
    return name_;
}
    
Node* Node::get_child(const std::string& child_name) const
{
    for(Node* child : children_)
    {
        if(child->name() == child_name)
        {
            return child;
        }
    }
    return nullptr;
}

Node* Node::add_child(const std::string& point)
{
    Node* ret = new Node(point);
    children_.emplace_back(ret);
    return ret;
}

const std::function<Response(const Request&)>& Node::get_service(Method method) const
{
    int pos = static_cast<std::underlying_type<Method>::type>(method);
    if(!status_[pos]) throw -1;
    return services_[pos];
}

void Node::show(std::string prefix)
{
    prefix = prefix + name_ + '/';
    std::cout << "url: " << prefix << std::endl << "active services: ";
    for(int i = 0; i < ENUM_SIZE_METHOD; i++)
    {
        if(status_[i])
        {
            std::cout << Request::str_method_[i] << " ";
        }
    }
    std::cout << std::endl << std::endl;
    for(auto child : children_)
    {
        child->show(prefix);
    }
}