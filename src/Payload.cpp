#include "Payload.hpp"

std::vector<std::string> Request::str_method_ = {
    "GET", "POST", "PUT", "DELETE"
};

std::string Response::version_ = "HTTP/1.1";
std::vector<std::string> Response::str_code_ = {"200", "404"};
std::vector<std::string> Response::str_message_ = {"OK", "Not Found"};
std::vector<std::string> Response::str_content_type_ = {
    "text/html", "text/plain", "image/jpeg", "image/png"
};

void Request::set_method(Method method)
{
    method_ = method;
}

Method Request::method() const
{
    return method_;
}

void Request::set_url(std::string url)
{
    url_ = url;
}

std::string Request::url() const
{
    return url_;
}

void Request::put(std::string key, std::string value)
{
    data_[key] = value;
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
        body_(body), content_type_(content_type), status_(status) {}

Status Response::status() const
{
    return status_;
}

std::string Response::body() const
{
    return body_;
}

ContentType Response::content_type() const
{
    return content_type_;
}