#include "ConnectionManager.h"

ConnectionManager::ConnectionManager()
{
}

void ConnectionManager::Start(ConnectionPtr c)
{
    connections.insert(c);
    c->Start();
}

void ConnectionManager::Stop(ConnectionPtr c)
{
    connections.erase(c);
    c->Stop();
}

void ConnectionManager::StopAll()
{
    for (auto c : connections)
    {
        c->Stop();
    }
    connections.clear();
}
