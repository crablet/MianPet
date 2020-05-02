#include <iostream>
#include <exception>

#include "Server.h"

int main()
{
    try
    {
        Server server;
        server.Run();
    }
    catch (std::exception &e)
    {
        std::cerr << "exception: " << e.what() << '\n';
    }

    return 0;
}
