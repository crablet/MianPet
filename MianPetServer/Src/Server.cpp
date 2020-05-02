#include "Server.h"

Server::Server()
    : ioContext(1),
      signals(ioContext),
      accepter(ioContext),
      connectionManager()
{
    signals.add(SIGINT);
    signals.add(SIGTERM);
#ifdef SIGQUIT
    signals.add(SIGQUIT);
#endif // SIGQUIT
    DoAwaitStop();
}

void Server::Run()
{
}

void Server::DoAccept()
{
}

void Server::DoAwaitStop()
{
    signals.async_wait(
    [this]([[maybe_unused]] std::error_code ec, [[maybe_unused]] int signo)
    {
        accepter.close();
        connectionManager.StopAll();
    });
}
