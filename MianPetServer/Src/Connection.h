#pragma once

#include <memory>
#include <array>
#include <string>
#include <utility>
#include <system_error>
#include <cinttypes>
#include <thread>

#include "asio.hpp"
#include "simdjson.h"

#include "Config/NetworkConstants.h"

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

private:
    asio::ip::tcp::socket socket;
    
    ConnectionManager& connectionManager;

    std::array<char, 8192> buffer;

    std::string reply;

    inline static simdjson::dom::parser parser{};
};

using ConnectionPtr = std::shared_ptr<Connection>;
