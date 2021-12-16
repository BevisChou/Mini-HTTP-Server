#include "Service.hpp"

void deploy(Mapping& mapping)
{
    mapping.add_service("/html/dopost", Method::POST, example);
}

Response example(const Request& request)
{
    std::string username = request.get("login");
    std::string password = request.get("pass");

    std::string begin = "<html><body>";
    std::string end = "</body></html>";

    std::string body = 
        begin + 
        (username == "3180102016" && password == "2016" ? 
            "Success!" : "Failure!") + 
        end;

    return Response(body, ContentType::TEXT_HTML);
}