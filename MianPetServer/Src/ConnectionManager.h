#pragma once

#include <unordered_set>

#include "Connection.h"

class ConnectionManager
{
public:
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager();

    void Start(ConnectionPtr c);
    void Stop(ConnectionPtr c);
    void StopAll();

private:
    std::unordered_set<ConnectionPtr> connections;
};
