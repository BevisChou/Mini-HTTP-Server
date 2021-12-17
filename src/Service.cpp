#include "Service.hpp"

// Please deploy your services here.
void deploy(Mapping& mapping)
{
    mapping.add_service("/", Method::GET, welcome);
    mapping.add_service("/html/dopost", Method::POST, html_dopost);
    mapping.add_service("/html/test.html", Method::GET, html_test);
    mapping.add_service("/html/img/logo.jpg", Method::GET, img_logo);
    mapping.add_service("/img/logo.jpg", Method::GET, img_logo);
    mapping.add_service("/txt/test.txt", Method::GET, txt_test);
}

// Please implement your services here.
Response welcome(const Request& request)
{
    std::string body = Util::http_wrapping("Welcome!");
    return Response(body, ContentType::TEXT_HTML);
}

Response html_dopost(const Request& request)
{
    std::string username = request.get("login");
    std::string password = request.get("pass");

    std::string body = Util::http_wrapping(
        (username == "user" && password == "pass" ? "Success!" : "Failure!"));

    return Response(body, ContentType::TEXT_HTML);
}

Response html_test(const Request& request)
{
    std::string content = Util::read_file(
        "/data/html/test.html");
    return Response(content, ContentType::TEXT_HTML);
}

Response img_logo(const Request& request)
{
    std::string content = Util::read_file(
        "/data/img/logo.jpg");
    return Response(content, ContentType::IMAGE_JPEG);
}

Response txt_test(const Request& request)
{
    std::string content = Util::read_file(
        "/data/txt/test.txt");
    return Response(content, ContentType::TEXT_PLAIN);
}