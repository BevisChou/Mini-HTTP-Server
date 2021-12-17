#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class Util {
public:
    static std::string http_wrapping(const std::string&);
    static std::string read_file(const std::string&);
    static std::vector<std::string> split(const std::string&, char);
};