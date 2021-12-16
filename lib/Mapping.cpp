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
    get_endpoint(url)->set_service(method, service);
}

std::function<Response(const Request&)> Mapping::get_service(
    std::string url, 
    Method method) const
{
    return get_endpoint(url)->get_service(method);
}

void Mapping::add_file_path(
    std::string url, 
    Method method, 
    std::string file_path)
{
    get_endpoint(url)->set_file(method, file_path);
}

std::string Mapping::get_file_path(std::string url, Method method) const
{
    return get_endpoint(url)->get_file_path(method);
}

const std::string& Node::get_file_path(Method method) const
{
    return resources_[std::underlying_type<Method>::type(method)].second;
}

void Mapping::remove(std::string url, Method method)
{
    get_endpoint(url)->remove_resource(method);
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
    std::vector<std::string> ret;
    int pos;
    do {
        url.erase(0);
        pos = url.find('/');
        ret.emplace_back(url.substr(0, pos));
        url.erase(0, pos);
    } while (pos != -1);
    return ret;
}

// Node

Node::Node(std::string name) :
    name_(name),
    resource_types_(ENUM_SIZE_METHOD, ResourceType::NONE),
    resources_(ENUM_SIZE_METHOD) {}

Node::~Node()
{
    for(Node* child : children_)
    {
        delete child;
    }
}

void Node::set_service(Method method, std::function<Response(const Request&)> service)
{
    resource_types_[std::underlying_type<Method>::type(method)] = ResourceType::SERVICE;
    resources_[std::underlying_type<Method>::type(method)].first = service;
    resources_[std::underlying_type<Method>::type(method)].second = "";
}

void Node::set_file(Method method, std::string file_path)
{
    resource_types_[std::underlying_type<Method>::type(method)] = ResourceType::FILE;
    resources_[std::underlying_type<Method>::type(method)].first = file_handler;
    resources_[std::underlying_type<Method>::type(method)].second = file_path;
}

void Node::remove_resource(Method method)
{ 
    resource_types_[std::underlying_type<Method>::type(method)] = ResourceType::NONE;
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
    return resources_[std::underlying_type<Method>::type(method)].first;
}

// file_handler
Response file_handler(const Request& request)
{
    // Ignore the key-value pairs which might further determine which file to send.

    std::string file_path = mapping_.get_file_path(request.url(), request.method());

    std::string content;
    ContentType content_type;
    Status status;
    
    // Content type
    // TODO

    std::ifstream fin(file_path, std::ios::binary);

    if(fin.is_open())
    {
        std::stringstream ss;
        ss << fin.rdbuf();
        content = ss.str();
        status = Status::REQUEST_SUCCEEDED;
    }
    else
    {
        status = Status::PAGE_NOT_FOUND;
    }

    return Response(content, content_type, status);
}