#include "Util.hpp"

std::string Util::http_wrapping(const std::string& message)
{
    std::string begin = "<html><body>";
    std::string end = "</body></html>";

    std::string body = begin + message + end;
    return body;
}

std::string Util::read_file(const std::string& path)
{
    std::string content;
    std::ifstream fin(path, std::ios::binary);

    if(fin.is_open())
    {
        std::stringstream ss;
        ss << fin.rdbuf();
        content = ss.str();
    }
    else
    {
        throw -1;
    }

    return content;
}

std::vector<std::string> Util::split(const std::string& str, char c)
{
    std::string s(str);
    s += c;
    std::vector<std::string> ret;
    int pos;
    while(true) {
        pos = s.find(c);
        if(pos == -1 || pos == 0) break;
        ret.emplace_back(s.substr(0, pos));

        s.erase(0, pos + 1);
    }
    return ret;
}