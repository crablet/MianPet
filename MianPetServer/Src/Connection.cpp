﻿#include "Connection.h"
#include "ConnectionManager.h"

Connection::Connection(asio::ip::tcp::socket socket, ConnectionManager &manager)
    : socket(std::move(socket)),
      connectionManager(manager)
{
}

void Connection::Start()
{
    DoRead();
}

void Connection::Stop()
{
    socket.close();
}

void Connection::DoRead()
{
    auto self(shared_from_this());
    socket.async_read_some(asio::buffer(buffer), 
    [this, self](std::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            TaskRunnerThread(length);
        }
        else if (ec != asio::error::operation_aborted)
        {
            connectionManager.Stop(shared_from_this());
        }
    });
}

void Connection::DoWrite()
{
    auto self(shared_from_this());
    asio::async_write(socket, asio::buffer(reply),
    [this, self](std::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            asio::error_code ignore;
            socket.shutdown(asio::socket_base::shutdown_both, ignore);
        }

        if (ec != asio::error::operation_aborted)
        {
            connectionManager.Stop(shared_from_this());
        }
    });
}

void Connection::TaskRunnerThread(int jsonLength)
{
    std::thread taskRunner(
    [&]()
    {
        try
        {
            const auto json = parser.parse(buffer.data(), jsonLength).get<simdjson::dom::object>();
            const char *id = json["id"].get<const char*>();
            const char *randomKey = json["random_key"].get<const char *>();
            const auto version = static_cast<std::int64_t>(json["version"]);
            const auto method = static_cast<std::int64_t>(json["method"]);

            if (method == GET)
            {
                const auto payload = json["payload"];
                const auto hint = static_cast<std::int64_t>(json["hint"]);
                if (hint == FOOD_SHOP_INFO)
                {

                }
                else if (hint == CLEAN_SHOP_INFO)
                {

                }
                else if (hint == PETPROFILE)
                {
                    DealWithGetPetProfile(id, randomKey);
                }
                else if (hint == CORE_KEY_FOR_PASSWORD_TRANSPORTATION)
                {
                    DealWithGetCoreKeyForPasswordTransportation(id);
                }
                else if (hint == LOGIN)
                {
                    const char *password = payload["password"].get<const char*>();
                    DealWithGetLogin(id, password, randomKey);
                }
                else
                {
                    // error
                }
            }
            else if (method == HERATBEAT)
            {
                DealWithHeartbeat(id, randomKey);
            }
            else if (method == LOGOUT)
            {
                DealWithLogout(id, randomKey);
            }
            else
            {
                // error
            }
        }
        catch (const simdjson::simdjson_error &err)
        {
            std::cout << err.what() << std::endl;
        }
    });
    taskRunner.detach();
}

void Connection::DealWithGetCoreKeyForPasswordTransportation(const char *id)
{
    try
    {
        std::lock_guard<std::mutex> lock(dbMutex);

        constexpr const char *sqlStr =
            R"(SELECT online
               FROM userinfo
               WHERE id = :id<char[16]>)";
        otl_stream selectStream(64, sqlStr, db);
        selectStream << id;

        int online;
        for (auto &r : selectStream)
        {
            r << online;
        }

        // 首先看该用户是否已经在线
        if (online) // 如果已经在线，则不允许重新登录，所以返回错误
        {
            reply = R"({"status":"failed"})";
        }
        else        // 如果没有在线，则根据规则随机生成corekey并且返回
        {
            std::random_device rd;
            std::minstd_rand gen(rd());
            std::uniform_int_distribution<> dis(100000, 999999);

            const auto key = std::to_string(dis(gen));
            tempKeyOf[id] = key;

            reply = R"({"corekey":")" + key + R"("})";
        }

        DoWrite();
    }
    catch (const otl_exception &exp)
    {
        std::cout << exp.stm_text << std::endl;
        std::cout << exp.msg << std::endl;
    }
}

void Connection::DealWithGetLogin(const char *id, const char *password, const char *randomKey)
{
    // TODO: 这一块似乎有bug，现在无法登出了。时而可以复现，时而不行，暂无100%复现的方法。

    try
    {
        std::lock_guard<std::mutex> lock(dbMutex);

        constexpr const char *sqlStr =
            R"(SELECT password, online, secretkey
               FROM userinfo
               WHERE id = :id<char[16]>)";
        otl_stream selectStream(64, sqlStr, db);
        selectStream << id;

        char truePassword[32 + 1];
        int online;
        char trueSecretKey[18 + 1];
        for (auto &in : selectStream)
        {
            in >> truePassword >> online >> trueSecretKey;
        }

        // 首先看密码是否正确
        if (std::strncmp(password, truePassword, 32) == 0)  // 如果密码正确，则看用户是否在线
        {
            if (online) // 如果用户已经在线，则不允许重新登陆，所以返回错误
            {
                reply = R"({"status":"failed"})";
            }
            else        // 如果用户还没有在线，才可以执行下面的登录流程
            {
                // TODO: need an extra compare between screctkey and tempKeyOf[id]

                constexpr const char *updateSecretKeySql =
                    R"(UPDATE userinfo
                       SET logintime = NOW(), online = 1, secretkey = :key<char[24]>, heartbeat = NOW()
                       WHERE id = :id<char[16]>)";
                otl_stream updateStream(1, updateSecretKeySql, db);
                updateStream << randomKey << id;

                reply = R"({"status":"success"})";
            }
        }
        else    // 如果用户密码错误，则返回失败
        {
            reply = R"({"status":"failed"})";
        }

        DoWrite();
    }
    catch (const otl_exception &exp)
    {
        std::cout << exp.stm_text << std::endl;
        std::cout << exp.msg << std::endl;
    }
}

void Connection::DealWithGetPetProfile(const char *id, const char *randomKey)
{
    try
    {
        std::lock_guard<std::mutex> lock(dbMutex);

        constexpr const char *checkOnlineAndSecretKeySqlStr =
            R"(SELECT online, secretKey FROM userinfo
               WHERE id = :id<char[16]>)";
        otl_stream checkOnlineStream(64, checkOnlineAndSecretKeySqlStr, db);
        checkOnlineStream << id;
        
        int online;
        char trueSecretKey[18 + 1];
        for (auto &r : checkOnlineStream)
        {
            r >> online >> trueSecretKey;
        }

        // 首先看密码是否正确
        if (online)
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果密码正确，则直接取出结果返回
            {
                constexpr const char *petprofileSql =
                    R"(SELECT id, username, level, age, growth, food, clean, health, mood, growth_speed, status, online_time
                       FROM petprofile
                       WHERE id = :id<char[16]>)";
                otl_stream petprofileStream(384, petprofileSql, db);
                petprofileStream << id;

                char id[16 + 1];
                char username[24 + 1];
                int level, age, growth, food, clean, health, mood, growthSpeed;
                char status[24 + 1];
                int onlineTime;
                for (auto &r : petprofileStream)    // 从数据库中取出petprofile数据
                {
                    r >> id >> username >> level >> age >> growth 
                      >> food >> clean >> health >> mood >> growthSpeed 
                      >> status >> onlineTime;
                }

                char buffer[384];
                std::snprintf
                (
                    buffer, 384,
                    R"({"id":"%s","username":"%s","level":%d,"age":%d,"growth":%d,"food":%d,"clean":%d,"health":%d,"mood":%d,"growth_speed":%d,"status":"%s","online_time":%d})",
                    id, username, level, age, growth, food, clean, health, mood, growthSpeed, status, onlineTime
                );
                reply = buffer;
            }
            else
            {
                reply = R"({"status":"failed"})";
            }
        }
        else
        {
            reply = R"({"status":"failed"})";
        }

        DoWrite();
    }
    catch (const otl_exception &exp)
    {
        std::cout << exp.stm_text << std::endl;
        std::cout << exp.msg << std::endl;
    }
}

void Connection::DealWithHeartbeat(const char *id, const char *randomKey)
{
    try
    {
        std::lock_guard<std::mutex> lock(dbMutex);

        constexpr const char *checkOnlineAndSecretKeySqlStr =
            R"(SELECT online, secretKey FROM userinfo
               WHERE id = :id<char[16]>)";
        otl_stream checkOnlineStream(64, checkOnlineAndSecretKeySqlStr, db);
        checkOnlineStream << id;

        int online;
        char trueSecretKey[18 + 1];
        for (auto &r : checkOnlineStream)
        {
            r >> online >> trueSecretKey;
        }

        // 首先看用户是否在线
        if (!online)    // 如果不在线，则报告错误
        {
            // error
        }
        else            // 如果用户在线，则判断randomKey是否一致，这里有简单的cookie的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) != 0)    // 如果randomKey不一致，则报告错误
            {
                // error
            }
            else        // 如果randomKey一致，则可以处理该心跳包
            {
                constexpr const char *countMinuteDeltaSql =
                    R"(SELECT TIMESTAMPDIFF(MINUTE, heartbeat, NOW())
                       FROM userinfo
                       WHERE id = :id<char[16]>)";
                otl_stream countMinuteDeltaStream(16, countMinuteDeltaSql, db);
                countMinuteDeltaStream << id;

                int minuteDelta;
                for (auto &r : countMinuteDeltaStream)
                {
                    r >> minuteDelta;
                }

                constexpr const char *updateHeartbeatSql =
                    R"(UPDATE userinfo
                       SET heartbeat = NOW()
                       WHERE id = :id<char[16]>)";
                otl_stream updateHeartbeatStream(1, updateHeartbeatSql, db);
                updateHeartbeatStream << id;

                constexpr const char *petprofileSql =
                    R"(SELECT level, age, growth, food, clean, health, mood, growth_speed, status, online_time
                       FROM petprofile
                       WHERE id = :id<char[16]>)";
                otl_stream petprofileStream(384, petprofileSql, db);
                petprofileStream << id;

                int level, age, growth, food, clean, health, mood, growthSpeed;
                char status[24 + 1];
                int onlineTime;
                for (auto &r : petprofileStream)
                {
                    r >> level >> age >> growth
                      >> food >> clean >> health >> mood >> growthSpeed
                      >> status >> onlineTime;
                }

                const auto newAge = age + growthSpeed * minuteDelta;
                const auto newGrowth = growth + growthSpeed * minuteDelta;
                const auto newLevel =   // 根据文档得出的所需经验计算公式，加号后面为布尔值
                    level + (newGrowth >= 
                                ((level % 2) == 0 ? (1 + level / 2) * level / 2 
                                                  : ((level + 1) / 2) * ((level + 1) / 2)));
                // TODO: finish the calculation
                const auto newFood = food;
                const auto newClean = clean;
                const auto newHealth = health;
                const auto newMood = mood;
                const auto newGrowthSpeed = growthSpeed;
                char newStatus[24 + 1];
                std::strcpy(newStatus, status);
                // TODO: finish the calculation

                const auto newOnlineTime = onlineTime + minuteDelta;

                constexpr const char *updateProfileSql =
                    R"(UPDATE petprofile
                       SET level = :newLevel<int>, age = :newAge<int>, growth = :newGrowth<int>, 
                           food = :newFood<int>, clean = :newClean<int>, health = :newHealth<int>, mood = :newMood<int>, growth_speed = :newGrowthSpeed<int>, 
                           status = :newStatus<char[24]>, online_time = :newOnlineTime<int>)";
                otl_stream updateProfileStream(1, updateProfileSql, db);
                updateProfileStream << newLevel << newAge << newGrowth 
                                    << newFood << newClean << newHealth << newMood << newGrowthSpeed
                                    << newStatus << newOnlineTime;
            }
        }
    }
    catch (const otl_exception &exp)
    {
        std::cout << exp.stm_text << std::endl;
        std::cout << exp.msg << std::endl;
    }
}

void Connection::DealWithLogout(const char *id, const char *randomKey)
{
    // TODO: 这一块似乎有bug，现在无法登出了。时而可以复现，时而不行，暂无100%复现的方法。

    try
    {
        std::lock_guard<std::mutex> lock(dbMutex);

        constexpr const char *checkOnlineAndSecretKeySqlStr =
            R"(SELECT online, secretKey FROM userinfo
               WHERE id = :id<char[16]>)";
        otl_stream checkOnlineStream(64, checkOnlineAndSecretKeySqlStr, db);
        checkOnlineStream << id;

        int online;
        char trueSecretKey[18 + 1];
        for (auto &r : checkOnlineStream)
        {
            r >> online >> trueSecretKey;
        }

        // 先检查用户是否在线，如果不在线那执行登出操作也没有意义
        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始登出
            {
                // 登出操作只需要更改数据库userinfo表中相应id的online为0和secretkey为NULL即可

                constexpr const char *updateSql =
                    R"(UPDATE userinfo
                       SET online = 0, secretkey = NULL
                       WHERE id = :id<char[16]>)";
                otl_stream updateStream(1, updateSql, db);
                updateStream << id;
            }
            else
            {
                // error
            }
        }
        else
        {
            // error
        }
    }
    catch (const otl_exception &exp)
    {
        std::cout << exp.stm_text << std::endl;
        std::cout << exp.msg << std::endl;
    }
}
