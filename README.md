# Mini HTTP Server

## Quick Start

Here's an example for developing a server-side application.

First, specify the port in `src/main.cpp`.

```c++
const int PORT = 50000;
```

Second, add the signature of the function in `src/Service.hpp`. Note that all functions you implement should accept an object of type `const Request&`, and return an object of type `Response`·

```c++
#include "Util.hpp"
#include "Mapping.hpp"

void deploy(Mapping& mapping);

// Below are custom functions.
Response welcome(const Request& request);
```

Third, implement your function and specify its route in `src/Service.cpp`.

```c++
#include "Service.hpp"

// Please deploy your services here.
void deploy(Mapping& mapping)
{
    mapping.add_service("/", Method::GET, welcome);
}

// Please implement your services here.
Response welcome(const Request& request)
{
    std::string body = Util::http_wrapping("Welcome!");
    return Response(body, ContentType::TEXT_HTML);
}
```

## Features

- Energy-efficient because busy waiting is waited by using facilities like `std::condition_variable`.
- Number of service threads could be specified.
- URL parameters would be parsed and processed so that developers could get them from `Request` object easily.
- Utilities functions are provided, e.g. function for reading files.

## TODOs

- Implement URL decoder.
- Rearrange folder structure.
- Add more utilities including decoder/encoder and more.
- Enrich documentation.
- Improve routing mechanism.
- ...
