#include "Connection.h"
#include "ConnectionManager.h"

Connection::Connection(asio::ip::tcp::socket socket, ConnectionManager &manager)
    : socket(std::move(socket)),
      connectionManager(manager)
{
}

void Connection::Start()
{
    DoRead();
}

void Connection::Stop()
{
    socket.close();
}

void Connection::DoRead()
{
    auto self(shared_from_this());
    socket.async_read_some(asio::buffer(buffer), 
    [this, self](std::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            TaskRunnerThread(length);
        }
        else if (ec != asio::error::operation_aborted)
        {
            connectionManager.Stop(shared_from_this());
        }
    });
}

void Connection::DoWrite()
{
    auto self(shared_from_this());
    asio::async_write(socket, asio::buffer(reply),
    [this, self](std::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            asio::error_code ignore;
            socket.shutdown(asio::socket_base::shutdown_both, ignore);
        }

        if (ec != asio::error::operation_aborted)
        {
            connectionManager.Stop(shared_from_this());
        }
    });
}

void Connection::TaskRunnerThread(int jsonLength)
{
    std::thread taskRunner(
    [&]()
    {
        try
        {
            const auto json = parser.parse(buffer.data(), jsonLength).get<simdjson::dom::object>();
            const auto id = std::string(json["id"]);
            const auto version = static_cast<std::int64_t>(json["version"]);
            const auto method = static_cast<std::int64_t>(json["method"]);

            if (method == GET)
            {
                const auto payload = json["payload"];
                const auto hint = static_cast<std::int64_t>(json["hint"]);
                if (hint == FOOD_SHOP_INFO)
                {

                }
                else if (hint == CLEAN_SHOP_INFO)
                {

                }
                else if (hint == PETPROFILE)
                {

                }
                else if (hint == CORE_KEY_FOR_PASSWORD_TRANSPORTATION)
                {

                }
                else if (hint == LOGIN)
                {

                }
                else
                {
                    // error
                }
            }
            else if (method == HERATBEAT)
            {

            }
            else if (method == LOGOUT)
            {

            }
            else
            {
                // error
            }
            reply = id + std::to_string(version) + std::to_string(method);

            DoWrite();
        }
        catch (const simdjson::simdjson_error &err)
        {
            std::cout << err.what() << std::endl;
        }
    });
    taskRunner.detach();
}
