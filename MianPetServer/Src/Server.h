#pragma once

#include <csignal>
#include <system_error>
#include <utility>

#include <asio.hpp>

#include "Connection.h"
#include "ConnectionManager.h"

class Server
{
public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server();

    void Run();

private:
    void DoAccept();
    void DoAwaitStop();

private:
    asio::io_context ioContext;
    asio::signal_set signals;
    asio::ip::tcp::acceptor acceptor;
    
    ConnectionManager connectionManager;
};
