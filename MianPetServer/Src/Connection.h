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
#include <vector>

#include "asio.hpp"
#include "simdjson.h"

#include "Config/NetworkConstants.h"

#include "DatabaseSharedVars.h"

// 如果开启了DEBUG，则会出现更多的调试性的输出
#define DEBUG

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    explicit Connection(
        asio::ip::tcp::socket socket, ConnectionManager &manager);

    // 开始接收tcp消息
    void Start();
    // 停止接收tcp消息
    void Stop();

private:
    // 用于向客户端接收消息
    void DoRead();
    // 用于向客户端发送消息
    void DoWrite();

    // 用于处理json文件所指定的任务，其中会解析json文件并分发任务至各个线程
    void TaskRunnerThread(int jsonLength);

    // 处理用于登录的corekey的请求 /GET/CORE_KEY_FOR_PASSWORD_TRANSPORTATION
    void DealWithGetCoreKeyForPasswordTransportation(const char *id);
    // 处理登入请求 /GET/LOGIN
    void DealWithGetLogin(const char *id, const char *password, const char *randomKey);
    // 处理获取宠物信息的请求 /GET/PETPROFILE
    void DealWithGetPetProfile(const char *id, const char *randomKey);

    // 处理心跳包 /HEARTBEAT
    void DealWithHeartbeat(const char *id, const char *randomKey);
    // 处理登出请求 /GET/LOGOUT
    void DealWithLogout(const char *id, const char *randomKey);

    // 处理食品商店信息请求 /GET/FOOD_SHOP_INFO
    void DealWithFoodShopInfo(const char *id, const char *randomKey, const std::vector<const char*> &items);
    // 处理日用品商店信息请求 /GET/CLEAN_SHOP_INFO
    void DealWithCleanShopInfo(const char *id, const char *randomKey, const std::vector<const char *> &items);
    // 处理购买食品/日用品的请求 /GET/BUY
    void DealWithBuy(const char *id, const char *randomKey, const char *item, int count);

private:
    asio::ip::tcp::socket socket;
    
    ConnectionManager& connectionManager;

    std::array<char, 8192> buffer;

    std::string reply;

    inline static std::unordered_map<std::string, std::string> tempKeyOf{};

    // 用于解释json文件
    // 根据simdjson文档，该变量应尽量复用
    inline static simdjson::dom::parser parser{};
};

using ConnectionPtr = std::shared_ptr<Connection>;
