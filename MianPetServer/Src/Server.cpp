#include "Server.h"

Server::Server()
    : ioContext(1),
      signals(ioContext),
      acceptor(ioContext),
      connectionManager()
{
    signals.add(SIGINT);
    signals.add(SIGTERM);
#ifdef SIGQUIT
    signals.add(SIGQUIT);
#endif // SIGQUIT
    DoAwaitStop();

    const auto endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 9999);
    acceptor.open(endpoint.protocol());
    acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen();
    DoAccept();
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
