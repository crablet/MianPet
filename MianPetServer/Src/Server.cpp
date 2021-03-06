﻿#include "Server.h"

Server::Server()
    : ioContext(1),
      signals(ioContext),
      acceptor(ioContext),
      connectionManager()
{
    try
    {
        otl_connect::otl_initialize(1); // 1 for multi-threaded
        db.rlogon("DSN=TEST");
    }
    catch (otl_exception &p)
    {
        std::cerr << p.msg << std::endl; // print out error message
        std::cerr << p.stm_text << std::endl; // print out SQL that caused the error
        std::cerr << p.sqlstate << std::endl; // print out SQLSTATE message
        std::cerr << p.var_info << std::endl; // print out the variable that caused the error
    }

    signals.add(SIGINT);
    signals.add(SIGTERM);
#ifdef SIGQUIT
    signals.add(SIGQUIT);
#endif // SIGQUIT
    DoAwaitStop();

    const auto endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), ServerPort);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen();
    DoAccept();

    CheckLogoutThread();

    std::cout << "The server is running." << std::endl;
}

Server::~Server()
{
    db.logoff();
}

void Server::Run()
{
    ioContext.run();
}

void Server::DoAccept()
{
    acceptor.async_accept(
    [this](std::error_code ec, asio::ip::tcp::socket socket)
    {
        if (!acceptor.is_open())
        {
            return;
        }

        if (!ec)
        {
            connectionManager.Start(
                std::make_shared<Connection>(
                    std::move(socket), connectionManager));
        }

        DoAccept();
    });
}

void Server::DoAwaitStop()
{
    signals.async_wait(
    [this]([[maybe_unused]] std::error_code ec, [[maybe_unused]] int signo)
    {
        acceptor.close();
        connectionManager.StopAll();
    });
}

void Server::CheckLogoutThread()
{
    std::thread checkLogoutThread(
    [this]()
    {
        while (true)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5min);

            try
            {
                std::lock_guard<std::mutex> lock(dbMutex);

                constexpr const char *updateSql =
                    R"(UPDATE userinfo
                       SET online = 0, heartbeat = NULL
                       WHERE id IN (SELECT id
                                    WHERE online = 1 AND TIMESTAMPDIFF(MINUTE, heartbeat, NOW()) >= 3))";
                otl_stream updateSqlStream(1, updateSql, db);
            }
            catch (const otl_exception &exp)
            {
                std::cout << exp.stm_text << std::endl;
                std::cout << exp.msg << std::endl;
            }
        }
    });
    checkLogoutThread.detach();
}
