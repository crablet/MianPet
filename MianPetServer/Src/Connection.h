#pragma once

#include <memory>
#include <array>
#include <string>
#include <utility>
#include <system_error>

#include "asio.hpp"
#include "simdjson.h"

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    explicit Connection(
        asio::ip::tcp::socket socket, ConnectionManager &manager);

    void Start();
    void Stop();

private:
    void DoRead();
    void DoWrite();

private:
    asio::ip::tcp::socket socket;
    
    ConnectionManager& connectionManager;

    std::array<char, 8192> buffer;

    std::string reply;

    inline static simdjson::dom::parser parser{};
};

using ConnectionPtr = std::shared_ptr<Connection>;
