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

#define OTL_ODBC // Compile OTL 4.0/ODBC
// The following #define is required with MyODBC 3.51.11 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
#include <otlv4.h> // include the OTL 4.0 header file

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

    otl_connect db;
};
