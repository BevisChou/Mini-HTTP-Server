#pragma once

#include <vector>
#include <string>
#include <unordered_map>

const int ENUM_SIZE_METHOD = 4;
enum class Method : uint8_t
{
    GET,
    POST,
    PUT, 
    DELETE
};

const int ENUM_SIZE_CONTENT_TYPE = 4;
enum class ContentType  : uint8_t
{
    TEXT_HTML,
    TEXT_PLAIN,
    IMAGE_JPEG,
    IMAGE_PNG
};

const int ENUM_SIZE_STATUS = 2;
enum class Status : uint8_t
{
    REQUEST_SUCCEEDED, 
    PAGE_NOT_FOUND
};

class Request {
public:
    void set_method(Method);
    Method method() const;
    void set_url(std::string);
    std::string url() const;
    void put(std::string, std::string);
    std::string get(std::string) const;
    ContentType content_type();
    std::string body();
    static std::vector<std::string> str_method_;
private:
    Method method_;
    std::string url_;
    std::unordered_map<std::string, std::string> data_;
    ContentType content_type_;
    std::string body_;
};

class Response {
public:
    Response(std::string, ContentType, Status = Status::REQUEST_SUCCEEDED);
    Status status() const;
    std::string body() const;
    ContentType content_type() const;
    static std::string version_;
    static std::vector<std::string> str_code_;
    static std::vector<std::string> str_message_;
    static std::vector<std::string> str_content_type_;
private:
    Status status_;
    std::string body_;
    ContentType content_type_;
};

/* About enum class
 * https://stackoverflow.com/questions/3069743/coding-conventions-naming-enums/3069863
 * https://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum
 */

/* About const variable in header files
 * https://stackoverflow.com/questions/12042549/define-constant-variables-in-c-header
 */