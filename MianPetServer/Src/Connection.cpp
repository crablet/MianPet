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

            }
            else if (method == LOGOUT)
            {

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

        if (online)
        {
            reply = R"({"status":"failed"})";
        }
        else
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

        if (std::strncmp(password, truePassword, 32) == 0)
        {
            if (online)
            {
                reply = R"({"status":"failed"})";
            }
            else
            {
                // TODO: need an extra compare between screctkey and tempKeyOf[id]

                constexpr const char *updateSecretKeySql =
                    R"(UPDATE userinfo
                       SET secretkey = :key<char[24]>, online = 1
                       WHERE id = :id<char[16]>)";
                otl_stream updateStream(1, updateSecretKeySql, db);
                updateStream << randomKey << id;

                reply = R"({"status":"success"})";
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

void Connection::DealWithGetPetProfile(const char *id, const char *randomKey)
{
    //try
    //{
    //    std::lock_guard<std::mutex> lock(dbMutex);

    //    constexpr const char *checkOnlineAndSecretKeySqlStr =
    //        R"(SELECT online, secretKey FROM userinfo
    //           WHERE id = :id<char[16]>)";
    //    otl_stream checkOnlineStream(64, checkOnlineAndSecretKeySqlStr, db);
    //    checkOnlineStream << id;
    //    
    //    int online;
    //    char trueSecretKey[18 + 1];
    //    for (auto &r : checkOnlineStream)
    //    {
    //        r >> online >> trueSecretKey;
    //    }

    //    if (online)
    //    {
    //        if (std::strncmp(randomKey, trueSecretKey, 18) == 0)
    //        {
    //            constexpr const char *petprofileSql =
    //                R"(SELECT id, user, level, age, growth, food, clean, health, mood, growth_speed, status, online_time
    //                   FROM petprofile
    //                   WHERE id = :id<char[16]>)";
    //            otl_stream petprofileStream(384, petprofileSql, db);
    //            petprofileStream << id;

    //            char id[16 + 1];
    //            char username[24 + 1];
    //            int level, age, growth, food, clean, health, mood, growthSpeed;
    //            char status[24 + 1];
    //            int onlineTime;
    //            for (auto &r : petprofileStream)
    //            {
    //                r >> id >> username >> level >> age >> growth 
    //                  >> food >> clean >> health >> mood >> growthSpeed 
    //                  >> status >> onlineTime;
    //            }

    //            char buffer[384];
    //            std::snprintf
    //            (
    //                buffer, 384,
    //                R"("{"id":"%s","username":"%s","level":%d,"age":%d,"growth":%d,"food":%d,"clean":%d,"health":%d,"mood":%d,"growth_speed":%d,"status":"%s","online_time":%d}")",
    //                id, username, level, age, growth, food, clean, health, mood, growthSpeed, status, onlineTime
    //            );
    //            reply = buffer;
    //        }
    //        else
    //        {
    //            reply = R"({"status":"failed"})";
    //        }
    //    }
    //    else
    //    {
    //        reply = R"({"status":"failed"})";
    //    }

    //    DoWrite();
    //}
    //catch (const otl_exception &exp)
    //{
    //    std::cout << exp.stm_text << std::endl;
    //    std::cout << exp.msg << std::endl;
    //}
}