#include "Server.hpp"
#include "Service.hpp"

const int PORT = 50000;
const int POOL_SIZE = 10;

int main()
{
    Mapping mapping;
    deploy(mapping);

    Server server(PORT, POOL_SIZE, mapping);
    server.run();

    return 0;
}