#include <iostream>
#include <asio.hpp>

int main()
{
    asio::io_context io;

    asio::steady_timer t(io, asio::chrono::seconds(5));
    t.async_wait([](const asio::error_code &e)
    {
        std::cout << "Hello World!" << std::endl;
    });

    io.run();

    return 0;
}
