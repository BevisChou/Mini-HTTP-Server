#include "Payload.hpp"
#include "Util.hpp"

#include <cstring>
#include <fstream>
#include <sstream>

class Interpreter {
public:
    static Request string_to_request(const std::string&);
    static std::string response_to_string(const Response&);
    static bool is_valid_request(const std::string&);
    static bool is_complete_request(const std::string&);
    static void fill_request(Request&, const std::string&);
private:
    static std::pair<std::string, std::string> 
        get_key_value_pair(const std::string&);
    static std::vector<std::pair<std::string, std::string>> 
        get_key_value_pairs(const std::string&);
};