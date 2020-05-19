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
            const auto [json, err] = parser.parse(buffer.data(), length).get<simdjson::dom::object>();
            if (err)
            {
                std::cout << err << std::endl;

                return;
            }
            const auto id = std::string(json["id"]);
            const auto version = static_cast<std::int64_t>(json["version"]);
            const auto method = static_cast<std::int64_t>(json["method"]);
            reply = id + std::to_string(version) + std::to_string(method);
            // do something and do write
            DoWrite();
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
