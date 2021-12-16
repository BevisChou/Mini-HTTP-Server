#include "Payload.hpp"

#include <cstring>
#include <fstream>
#include <sstream>

class Interpreter {
public:
    static Request string_to_request(const std::string& request_str);
    static std::string response_to_string(const Response& response);
private:
};