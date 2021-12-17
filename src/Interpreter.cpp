#include "Interpreter.hpp"

Request Interpreter::string_to_request(const std::string& request_str)
{
    Request ret;

    std::string str(request_str);
    int pos = str.find(' ');
    std::string method = str.substr(0, pos);

    if(method == "GET") ret.set_method(Method::GET);
    else if(method == "POST") ret.set_method(Method::POST);
    else if(method == "PUT") ret.set_method(Method::PUT);
    else if(method == "DELETE") ret.set_method(Method::DELETE);

    str.erase(0, pos + 1);
    pos = str.find(' ');
    std::string query = str.substr(0, pos);
    int mark = query.find('?');

    std::string url;

    if(mark == -1)
    {
        url = query;
    }
    else
    {
        url = query.substr(0, mark);
        query.erase(0, mark + 1);
        fill_request(ret, query);
    }
    ret.set_url(url);

    std::string tail = "\r\n\r\n";
    pos = str.find(tail);
    str.erase(0, pos + tail.length());

    if(str.length() > 0)
    {
        fill_request(ret, str);
    }

    return ret;
}

std::string Interpreter::response_to_string(const Response& response)
{
    int pos;
    std::string body(response.body()), ret;
    pos = static_cast<std::underlying_type<Status>::type>(response.status());
    ret = ret + Response::version_ + " " + Response::str_code_[pos] + " "
        + Response::str_message_[pos] + '\n' + "Content-Type: ";
    pos = static_cast<std::underlying_type<ContentType>::type>(response.content_type());
    ret = ret +  Response::str_content_type_[pos] + '\n' + "Content-Length: " 
        + std::to_string(body.length()) + '\n' + '\n' + body;
    return ret;
}

bool Interpreter::is_valid_request(const std::string& request_str)
{
    if(request_str.length() == 0) return false;
    
    // Pending.
    return true;
}

bool Interpreter::is_complete_request(const std::string& request_str)
{
    std::string len_str(request_str), attr_str = "Content-Length: ";
    len_str.erase(0, len_str.find(attr_str) + attr_str.length());
    len_str.erase(len_str.find("\r\n"));
    int len = std::stoi(len_str);

    std::string header_tail_str = "\r\n\r\n";
    int header_tail = request_str.find(header_tail_str);
    return header_tail == request_str.length() - header_tail_str.length() - len;
}

void Interpreter::fill_request(Request& request, const std::string& str)
{
    auto pairs = get_key_value_pairs(str);
    for(auto& pair : pairs)
    {
        request.put(pair.first, pair.second);
    }
}

std::pair<std::string, std::string> Interpreter::get_key_value_pair(const std::string& str)
{
    std::vector<std::string> strs = Util::split(str, '=');
    if(strs.size() == 1)
    {
        strs.emplace_back("");
    }
    return std::pair<std::string, std::string>(strs[0], strs[1]);
}

std::vector<std::pair<std::string, std::string>> Interpreter::get_key_value_pairs(const std::string& str)
{
    std::vector<std::pair<std::string, std::string>> ret;
    std::vector<std::string> list = Util::split(str, '&');
    for(auto& str : list)
    {
        ret.emplace_back(get_key_value_pair(str));
    }
    return ret;
}