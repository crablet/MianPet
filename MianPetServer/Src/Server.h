#pragma once

#include <csignal>
#include <system_error>
#include <utility>
#include <iostream>
#include <string>

#include <asio.hpp>

#include "Connection.h"
#include "ConnectionManager.h"

#include "Config/NetworkConstants.h"

#include "DatabaseSharedVars.h"

class Server
{
public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server();
    ~Server();

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
