#include "Server.hpp"

const int PORT = 2016;
const int POOL_SIZE = 10;

int main()
{
    Server server(PORT, POOL_SIZE);
    server.run();
    return 0;
}