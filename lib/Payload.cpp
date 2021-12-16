#include "Payload.hpp"

Method Request::method() const
{
    return method_;
}

std::string Request::url() const
{
    return url_;
}

std::string Request::get(std::string key) const
{
    auto itor = data_.find(key);
    if(itor == data_.end())
    {
        throw -1;
    }
    return itor->second;
}

ContentType Request::content_type()
{
    return content_type_;
}

std::string Request::body()
{
    return body_;
}

Response::Response(
    std::string body, 
    ContentType content_type, 
    Status status) : 
        body_(body), content_type_(content_type_), status_(status) {}