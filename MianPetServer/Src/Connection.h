#pragma once

#include <memory>
#include <array>
#include <string>
#include <utility>
#include <system_error>
#include <cinttypes>
#include <thread>
#include <random>
#include <iostream>
#include <cstdio>
#include <unordered_map>

#include "asio.hpp"
#include "simdjson.h"

#include "Config/NetworkConstants.h"

#include "DatabaseSharedVars.h"

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

    // 用于处理json文件所指定的任务，其中会解析json文件并分发任务至各个线程
    void TaskRunnerThread(int jsonLength);

    void DealWithGetCoreKeyForPasswordTransportation(const char *id);
    void DealWithGetLogin(const char *id, const char *password, const char *randomKey);
    void DealWithGetPetProfile(const char *id, const char *randomKey);

    void DealWithHeartbeat(const char *id, const char *randomKey);
    void DealWithLogout(const char *id, const char *randomKey);

private:
    asio::ip::tcp::socket socket;
    
    ConnectionManager& connectionManager;

    std::array<char, 8192> buffer;

    std::string reply;

    inline static std::unordered_map<std::string, std::string> tempKeyOf{};

    inline static simdjson::dom::parser parser{};
};

using ConnectionPtr = std::shared_ptr<Connection>;
