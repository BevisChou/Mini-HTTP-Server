#include "Server.hpp"
#include "../src/Service.hpp"

const int PORT = 2016;
const int POOL_SIZE = 10;

Mapping mapping_;

int main()
{
    deploy(mapping_);

    Server server(PORT, POOL_SIZE, mapping_);
    server.run();
    return 0;
}