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
                    const auto items = payload["items"];
                    std::vector<const char*> itemsCharVector;
                    for (const auto &name : items)
                    {
                        itemsCharVector.push_back(name.get<const char*>());
                    }

                    DealWithFoodShopInfo(id, randomKey, itemsCharVector);
                }
                else if (hint == CLEAN_SHOP_INFO)
                {
                    const auto items = payload["items"];
                    std::vector<const char*> itemsCharVector;
                    for (const auto &name : items)
                    {
                        itemsCharVector.push_back(name.get<const char*>());
                    }

                    DealWithCleanShopInfo(id, randomKey, itemsCharVector);
                }
                else if (hint == BUY)
                {
                    const char *item = payload["item"].get<const char*>();
                    const auto count = static_cast<std::int64_t>(payload["count"]);
                    DealWithBuy(id, randomKey, item, count);
                }
                else if (hint == USE)
                {
                    const char *item = payload["item"].get<const char*>();
                    const auto count = static_cast<std::int64_t>(payload["count"]);
                    DealWithUse(id, randomKey, item, count);
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
                else if (hint == WORKBEGIN)
                {
                    const char *job = payload["job"].get<const char*>();
                    DealWithWorkBegin(id, randomKey, job);
                }
                else if (hint == WORKEND)
                {
                    const char *job = payload["job"].get<const char*>();
                    DealWithWorkEnd(id, randomKey, job);
                }
                else if (hint == JOBS_INFO)
                {
                    const auto jobs = payload["jobs"];
                    std::vector<const char*> jobsCharVector;
                    for (const auto &job : jobs)
                    {
                        jobsCharVector.push_back(job.get<const char*>());
                    }
                    DealWithJobsInfo(id, randomKey, jobsCharVector);
                }
                else if (hint == WORK_STATUS)
                {
                    DealWithWorkStatus(id, randomKey);
                }
                else if (hint == FOOD_VALUE)
                {
                    DealWithFoodValue(id, randomKey);
                }
                else if (hint == CLEAN_VALUE)
                {
                    DealWithCleanValue(id, randomKey);
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
#ifdef DEBUG
    std::cout << "In function Connection::DealWithGetCoreKeyForPasswordTransportation: " << id << std::endl;
#endif // DEBUG
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
            r >> online;
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
#ifdef DEBUG
    std::cout << "In function Connection::DealWithGetLogin: " << id << randomKey << std::endl;
#endif // DEBUG
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
                // TODO: screctkey和tempKeyOf[id]需要添加额外的比较

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
#ifdef DEBUG
    std::cout << "In function Connection::DealWithGetPetProfile: " << id << randomKey << std::endl;
#endif // DEBUG
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
                    R"(SELECT id, username, level, age, growth, food, clean, health, mood, growth_speed, status, online_time, tuotuo
                       FROM petprofile
                       WHERE id = :id<char[16]>)";
                otl_stream petprofileStream(384, petprofileSql, db);
                petprofileStream << id;

                char id[16 + 1];
                char username[24 + 1];
                int level, age, growth, food, clean, health, mood, growthSpeed, tuotuo;
                int status;
                int onlineTime;
                for (auto &r : petprofileStream)    // 从数据库中取出petprofile数据
                {
                    r >> id >> username >> level >> age >> growth 
                      >> food >> clean >> health >> mood >> growthSpeed 
                      >> status >> onlineTime >> tuotuo;
                }

                char jsonBuffer[400];
                std::snprintf
                (
                    jsonBuffer, sizeof(jsonBuffer),
                    R"({"id":"%s","username":"%s","level":%d,"age":%d,"growth":%d,"food":%d,"clean":%d,"health":%d,"mood":%d,"growth_speed":%d,"status":"%d","online_time":%d,"tuotuo":%d})",
                    id, username, level, age, growth, food, clean, health, mood, growthSpeed, status, onlineTime, tuotuo
                );
                reply = jsonBuffer;
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

#ifdef DEBUG
        std::cout << "In function Connection::DealWithGetPetProfile: reply = " << reply << std::endl;
#endif // DEBUG

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
#ifdef DEBUG
    std::cout << "In function Connection::DealWithHeartbeat: " << id << randomKey << std::endl;
#endif // DEBUG
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

                // TODO：更新打工状态，若正在打工，则需要关注此处
                constexpr const char *countWorkingJobsSql =
                    R"(SELECT COUNT(job) AS workingJobs
                       FROM workinginfo
                       WHERE id = :id<char[16]> AND working = 1)";
                otl_stream countWorkingJobsStream(4, countWorkingJobsSql, db);
                countWorkingJobsStream << id;

                int count;
                for (auto &r : countWorkingJobsStream)
                {
                    r >> count;
                }

                int tuotuoDelta = 0;
                if (count == 0)
                {
                    // 无正在打工的项目
                }
                else if (count == 1)
                {
                    constexpr const char *findStatusSql =
                        R"(SELECT job, TIMESTAMPDIFF(MINUTE, begintime, NOW())
                           FROM workinginfo
                           WHERE id = :id<char[16]>)";
                    otl_stream findStatusStream(64, findStatusSql, db);
                    findStatusStream << id;

                    char jobName[18 + 1];
                    int workingTime;
                    for (auto &r : findStatusStream)
                    {
                        r >> jobName >> workingTime;
                    }

                    if (workingTime >= 60)  // 打工一小时后即可停止
                    {
                        constexpr const char *getWageSql =
                            R"(SELECT wage
                               FROM jobsinfo
                               WHERE name = :name<char[18]>)";
                        otl_stream getWageStream(4, getWageSql, db);
                        getWageStream << jobName;

                        for (auto &r : getWageStream)
                        {
                            r >> tuotuoDelta;
                        }

                        constexpr const char *stopWorkingSql =
                            R"(UPDATE workinginfo
                               SET count = count + 1, working = 0
                               WHERE id = :id<char[16]> AND job = :job<char[18]>)";
                        otl_stream stopWorkingStream(1, stopWorkingSql, db);
                        stopWorkingStream << id << jobName;
                    }
                }
                else
                {
                    // error
                }

                constexpr const char *petprofileSql =
                    R"(SELECT level, age, growth, food, clean, health, mood, growth_speed, status, online_time, tuotuo
                       FROM petprofile
                       WHERE id = :id<char[16]>)";
                otl_stream petprofileStream(384, petprofileSql, db);
                petprofileStream << id;

                int level, age, growth, food, clean, health, mood, growthSpeed;
                int status;
                int onlineTime;
                int tuotuo;
                for (auto &r : petprofileStream)
                {
                    r >> level >> age >> growth
                      >> food >> clean >> health >> mood >> growthSpeed
                      >> status >> onlineTime >> tuotuo;
                }

                const auto newAge = age + minuteDelta;
                const auto newGrowth = growth + growthSpeed * minuteDelta;
                const auto newLevel =   // 根据文档得出的所需经验计算公式，加号后面为布尔值
                    level + (newGrowth >= 
                                ((level % 2) == 0 ? (1 + level / 2) * level / 2 
                                                  : ((level + 1) / 2) * ((level + 1) / 2)));
                // TODO: finish the calculation
                const auto newFood = food >= FoodNormalOrAbnormal ?
                                     food - FoodDeltaWhenNormal :
                                     food - FoodDeltaWhenAbnormal;
                const auto newClean = clean >= CleanNormalOrAbnormal ?
                                      clean - CleanDeltaWhenNormal :
                                      clean - CleanDeltaWhenAbnormal;
                const auto newHealth = health;
                const auto newMood = mood;
                const auto newGrowthSpeed = newMood >= 50 ? 2 : 1;
                const auto newStatus = status;
                // TODO: finish the calculation

                const auto newOnlineTime = onlineTime + minuteDelta;

                const auto newTuotuo = tuotuo + tuotuoDelta;

                constexpr const char *updateProfileSql =
                    R"(UPDATE petprofile
                       SET level = :newLevel<int>, age = :newAge<int>, growth = :newGrowth<int>, 
                           food = :newFood<int>, clean = :newClean<int>, health = :newHealth<int>, mood = :newMood<int>, growth_speed = :newGrowthSpeed<int>, 
                           status = :newStatus<int>, online_time = :newOnlineTime<int>, tuotuo = :newTuotuo<int>
                       WHERE id = :id<char[16]>)";
                otl_stream updateProfileStream(1, updateProfileSql, db);
                updateProfileStream << newLevel << newAge << newGrowth 
                                    << newFood << newClean << newHealth << newMood << newGrowthSpeed
                                    << newStatus << newOnlineTime << newTuotuo
                                    << id;
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
#ifdef DEBUG
    std::cout << "In function Connection::DealWithLogout: " << id << randomKey << std::endl;
#endif // DEBUG
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
                // 并且要把正在进行的状态比如打工停止

                // 停止打工

                constexpr const char *getWorkingStatusSql =
                        R"(SELECT job, TIMESTAMPDIFF(MINUTE, begintime, NOW())
                           FROM workinginfo
                           WHERE id = :id<char[16]> AND working = 1)";
                otl_stream getWorkingStatusStream(36, getWorkingStatusSql, db);
                getWorkingStatusStream << id;

                char jobName[36 + 1];
                int workingTime;
                for (auto &r : getWorkingStatusStream)
                {
                    r >> jobName >> workingTime;
                }

                constexpr const char *endWorkingSql =
                        R"(UPDATE workinginfo
                           SET working = 0
                           WHERE id = :id<char[16]> AND job = :job<char[36]>)";
                otl_stream endWorkingStream(1, endWorkingSql, db);
                endWorkingStream << id << jobName;

                if (workingTime >= 60)  // 暂定一次工作时间为60分钟，时薪为jobsinfo中的wage
                {
                    constexpr const char *getWageSql =
                            R"(SELECT wage
                               FROM jobsinfo
                               WHERE name = :jobName<char[18]>)";
                    otl_stream getWageStream(8, getWageSql, db);
                    getWageStream << jobName;

                    int wage;
                    for (auto &r : getWageStream)
                    {
                        r >> wage;
                    }

                    constexpr const char *updateTuotuoSql =
                            R"(UPDATE petprofile
                           SET tuotuo = tuotuo + :wage<int>
                           WHERE id = :id<char[16]>)";
                    otl_stream updateTuotuoStream(1, updateTuotuoSql, db);
                    updateTuotuoStream << wage << id;
                }

                constexpr const char *updateSql =
                    R"(UPDATE userinfo
                       SET online = 0, secretkey = NULL
                       WHERE id = :id<char[16]>)";
                otl_stream updateStream(1, updateSql, db);
                updateStream << id;

#ifdef DEBUG
                std::cout << "In function Connection::DealWithLogout: LOGOUT SECCEEDED!" << std::endl;
#endif
            }
            else
            {
                // error
#ifdef DEBUG
                std::cout << "In function Connection::DealWithLogout: LOGOUT FAILED! Reason: randomKey not matches secretKey" << std::endl;
#endif // DEBUG
            }
        }
        else
        {
            // error
#ifdef DEBUG
            std::cout << "In function Connection::DealWithLogout: LOGOUT FAILED! Reason: User is not online." << std::endl;
#endif // DEBUG
        }
    }
    catch (const otl_exception &exp)
    {
        std::cout << exp.stm_text << std::endl;
        std::cout << exp.msg << std::endl;
    }
}

void Connection::DealWithFoodShopInfo(const char *id, const char *randomKey, const std::vector<const char*> &items)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithFoodShopInfo: " << id << randomKey << std::endl;
#endif // DEBUG
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
                // 回复的json格式为：
                // {
                //   "items":
                //   [
                //     {
                //       "name": string,
                //       "amount": int,
                //       "food": int,
                //       "price": int
                //     },
                //     ...
                //   ]
                //  }
                std::string replyJson = R"({"items":[)";

                for (const auto &itemname : items)
                {
                    constexpr const char *selectItemQuantitySqlStr =
                        R"(SELECT quantity FROM ownitems
                           WHERE id = :id<char[16]> AND itemname = :itemname<char[18]>)";
                    otl_stream selectItemQuantityStream(64, selectItemQuantitySqlStr, db);
                    selectItemQuantityStream << id << itemname;

                    int amount;
                    for (auto &in : selectItemQuantityStream)
                    {
                        in >> amount;
                    }

                    constexpr const char *selectItemInfoSql =
                            R"(SELECT price, food
                               FROM shopinfo
                               WHERE itemname = :itemname<char[18]>)";
                    otl_stream selectItemInfoStream(64, selectItemInfoSql, db);
                    selectItemInfoStream << itemname;

                    int price, food;
                    for (auto &in : selectItemInfoStream)
                    {
                        in >> price >> food;
                    }

                    replyJson += R"({"name":")";
                    replyJson += itemname;
                    replyJson += R"(","amount":)";
                    replyJson += std::to_string(amount);
                    replyJson += R"(,"food":)";
                    replyJson += std::to_string(food);
                    replyJson += R"(,"price":)";
                    replyJson += std::to_string(price);
                    replyJson += R"(},)";
                }

                if (replyJson.back() == ',')
                {
                    replyJson.pop_back();   // 去除最后一个','
                }
                replyJson += R"(]})";
                reply = std::move(replyJson);

#ifdef DEBUG
                std::cout << "In function Connection::DealWithFoodShopInfo: "
                          << " reply: " << reply << '\n';
#endif

                DoWrite();
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

void Connection::DealWithCleanShopInfo(const char *id, const char *randomKey, const std::vector<const char*> &items)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithCleanShopInfo: " << id << randomKey << std::endl;
#endif // DEBUG
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
                // 回复的json格式为：
                // {
                //   "items":
                //   [
                //     {
                //       "name": string,
                //       "amount": int,
                //       "clean": int,
                //       "price": int
                //     },
                //     ...
                //   ]
                //  }
                std::string replyJson = R"({"items":[)";
                replyJson.reserve(64);

                for (const auto &itemname : items)
                {
                    constexpr const char *selectItemQuantitySqlStr =
                            R"(SELECT quantity FROM ownitems
                               WHERE id = :id<char[16]> AND itemname = :itemname<char[18]>)";
                    otl_stream selectItemQuantityStream(64, selectItemQuantitySqlStr, db);
                    selectItemQuantityStream << id << itemname;

                    int amount;
                    for (auto &in : selectItemQuantityStream)
                    {
                        in >> amount;
                    }

                    constexpr const char *selectItemInfoSql =
                            R"(SELECT price, clean
                               FROM shopinfo
                               WHERE itemname = :itemname<char[18]>)";
                    otl_stream selectItemInfoStream(64, selectItemInfoSql, db);
                    selectItemInfoStream << itemname;

                    int price, clean;
                    for (auto &in : selectItemInfoStream)
                    {
                        in >> price >> clean;
                    }

                    replyJson += R"({"name":")";
                    replyJson += itemname;
                    replyJson += R"(","amount":)";
                    replyJson += std::to_string(amount);
                    replyJson += R"(,"clean":)";
                    replyJson += std::to_string(clean);
                    replyJson += R"(,"price":)";
                    replyJson += std::to_string(price);
                    replyJson += R"(},)";
                }

                if (replyJson.back() == ',')
                {
                    replyJson.pop_back();   // 去除最后一个','
                }
                replyJson += R"(]})";
                reply = std::move(replyJson);

#ifdef DEBUG
                std::cout << "In function Connection::DealWithCleanShopInfo: "
                          << " reply: " << reply << '\n';
#endif

                DoWrite();
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

void Connection::DealWithBuy(const char *id, const char *randomKey, const char *item, int count)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithBuy: " << id << randomKey << std::endl;
#endif // DEBUG
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
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始处理购买请求
            {
                // 获取面宠拥有的坨坨数量
                constexpr const char *getTuotuoAmountSql =
                    R"(SELECT tuotuo FROM petprofile
                       WHERE id = :id<char[16]>)";
                otl_stream getTuotuoAmountStream(16, getTuotuoAmountSql, db);
                getTuotuoAmountStream << id;

                int tuotuoAmount;
                for (auto &r : getTuotuoAmountStream)
                {
                    r >> tuotuoAmount;
                }

                // 获取物品单价
                constexpr const char *getItemPriceSql =
                    R"(SELECT price FROM shopinfo
                       WHERE itemname = :item<char[18]>)";
                otl_stream getItemPriceStream(16, getItemPriceSql, db);
                getItemPriceStream << item;

                int itemPrice;
                for (auto &r : getItemPriceStream)
                {
                    r >> itemPrice;
                }

                // 共需要花费的坨坨总数
                const auto totalPrice = itemPrice * count;
                if (totalPrice > tuotuoAmount)  // 如果坨坨不够花，则失败
                {
                    reply = R"({"status":"failed"})";
                }
                else        // 坨坨够用
                {
                    // 使用后的坨坨余额
                    const auto currentTuotuo = tuotuoAmount - totalPrice;
                    constexpr const char *updateTuotuoSql =
                        R"(UPDATE petprofile
                           SET tuotuo = :currentTuotuo<int>
                           WHERE id = :id<char[16]>)";
                    otl_stream updateTuotuoStream(1, updateTuotuoSql, db);
                    updateTuotuoStream << currentTuotuo << id;

                    // 更新使用后的坨坨余额
                    constexpr const char *updateOwnItemsAmountSql =
                        R"(UPDATE ownitems
                           SET quantity = quantity + :delta<int>
                           WHERE id = :id<char[16]> AND itemname = :itemname<char[18]>)";
                    otl_stream updateOwnItemsAmountStream(1, updateOwnItemsAmountSql, db);
                    updateOwnItemsAmountStream << count << id << item;

                    reply = R"({"status":"succeeded"})";
                }

                DoWrite();
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

void Connection::DealWithUse(const char *id, const char *randomKey, const char *item, int count)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithUse: " << id << randomKey << std::endl;
#endif // DEBUG
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

        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始操作了
            {
                constexpr const char *selectItemInfoSqlStr =
                    R"(SELECT quantity FROM ownitems
                       WHERE id = :id<char[16]> AND itemname = :itemname<char[18]>)";
                otl_stream selectItemInfoStream(64, selectItemInfoSqlStr, db);
                selectItemInfoStream << id << item;

                int totalAmount; // 用户现在拥有该物品的个数
                for (auto &in : selectItemInfoStream)
                {
                    in >> totalAmount;
                }

                if (totalAmount >= count)   // 如果用户现在拥有的物品够用，则去使用
                {
                    // 使用物品，原拥有的物品要减少
                    constexpr const char *updateOwnItemsAmountSql =
                        R"(UPDATE ownitems
                           SET quantity = quantity - :delta<int>
                           WHERE id = :id<char[16]> AND itemname = :itemname<char[18]>)";
                    otl_stream updateOwnItemsAmountStream(1, updateOwnItemsAmountSql, db);
                    updateOwnItemsAmountStream << count << id << item;

                    // 获取这个物品能为面宠带来的属性提升
                    constexpr const char *getBonusSql =
                        R"(SELECT food, clean, health
                           FROM shopinfo
                           WHERE itemname = :itemname<char[18]>)";
                    otl_stream getBonusStream(32, getBonusSql, db);
                    getBonusStream << item;

                    int foodBonus, cleanBonus, healthBonus;
                    for (auto &r : getBonusStream)
                    {
                        r >> foodBonus >> cleanBonus >> healthBonus;
                    }

#ifdef DEBUG
                    std::cout << "In function Connection::DealWithUse: Use " << item
                              << ", foodBonus: " << foodBonus
                              << ", cleanBonus: " << cleanBonus
                              << ", healthBonus: " << healthBonus << '\n';
#endif

                    // 获取面宠原来的属性值
                    constexpr const char *getOriginalStatusSql =
                        R"(SELECT food, clean, health
                           FROM petprofile
                           WHERE id = :id<char[16]>)";
                    otl_stream getOriginalStatusStream(32, getOriginalStatusSql, db);
                    getOriginalStatusStream << id;

                    int food, clean, health;
                    for (auto &r : getOriginalStatusStream)
                    {
                        r >> food >> clean >> health;
                    }

                    // 计算使用后的属性值，最大值为3600
                    food = std::min(3600, food + foodBonus);
                    clean = std::min(3600, clean + cleanBonus);
                    health = std::min(100, health + healthBonus);

                    // 更新使用后的状态
                    constexpr const char *updatePetprofileSql =
                        R"(UPDATE petprofile
                           SET food = :food<int>, clean = :clean<int>, health = :health<int>
                           WHERE id = :id<char[16]>)";
                    otl_stream updatePetprofileStream(1, updatePetprofileSql, db);
                    updatePetprofileStream << food << clean << health << id;

                    reply = R"({"status":"succeeded"})";
                }
                else    // 用户现在拥有的物品不够用
                {
                    reply = R"({"status":"failed"})";
                }

                DoWrite();
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

void Connection::DealWithWorkBegin(const char *id, const char *randomKey, const char *job)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithWorkBegin: " << id << randomKey << std::endl;
#endif // DEBUG
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

        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始查询了
            {
                constexpr const char *countWorkingJobsSql =
                    R"(SELECT COUNT(job) AS workingJobs
                       FROM workinginfo
                       WHERE id = :id<char[16]> AND working = 1)";
                otl_stream countWorkingJobsStream(4, countWorkingJobsSql, db);
                countWorkingJobsStream << id;

                int count;
                for (auto &r : countWorkingJobsStream)
                {
                    r >> count;
                }

                if (count != 0)
                {
                    reply = R"({"status":"failed"})";
                }
                else
                {
                    constexpr const char *checkGoalJobStatusSql =
                        R"(SELECT working
                           FROM workinginfo
                           WHERE id = :id<char[16]> AND job = :job<char[18]>)";
                    otl_stream checkGoalJobStatusStream(4, checkGoalJobStatusSql, db);
                    checkGoalJobStatusStream << id << job;

                    int isAlreadyWorking;
                    for (auto &r : checkGoalJobStatusStream)
                    {
                        r >> isAlreadyWorking;
                    }
                    if (isAlreadyWorking)
                    {
                        reply = R"({"status":"failed"})";
                    }
                    else
                    {
                        constexpr const char *beginWorkingSql = 
                            R"(UPDATE workinginfo
                               SET begintime = NOW(), count = count + 1, working = 1
                               WHERE id = :id<char[16]> AND job = :job<char[18]>)";
                        otl_stream beginWorkingStream(1, beginWorkingSql, db);
                        beginWorkingStream << id << job;

                        reply = R"({"status":"succeeded"})";
                    }
                }

                DoWrite();
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

void Connection::DealWithWorkEnd(const char *id, const char *randomKey, const char *job)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithWorkEnd: " << id << randomKey << std::endl;
#endif // DEBUG

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

        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始查询了
            {
                constexpr const char *getWorkingStatusSql =
                    R"(SELECT job, TIMESTAMPDIFF(MINUTE, begintime, NOW())
                       FROM workinginfo
                       WHERE id = :id<char[16]> AND working = 1)";
                otl_stream getWorkingStatusStream(36, getWorkingStatusSql, db);
                getWorkingStatusStream << id;

                char jobName[36 + 1];
                int workingTime;
                if (getWorkingStatusStream.eof())   // 如果还没开始打工就会进入这个if，因为获取不到任何打工信息，eof为真
                {
                    reply = R"({"status":"work not begins"})";
                    DoWrite();

                    return;
                }
                else
                {
                    for (auto &r : getWorkingStatusStream)
                    {
                        r >> jobName >> workingTime;
                    }
                }

#ifdef DEBUG
                std::cout << "In function Connection::DealWithWorkEnd: " << id
                          << " job name: " << jobName
                          << " working time: " << workingTime;
#endif // DEBUG

                constexpr const char *endWorkingSql =
                        R"(UPDATE workinginfo
                           SET working = 0
                           WHERE id = :id<char[16]> AND job = :job<char[36]>)";
                otl_stream endWorkingStream(1, endWorkingSql, db);
                endWorkingStream << id << jobName;

                if (workingTime >= 60)  // 暂定一次工作时间为60分钟，时薪为jobsinfo中的wage
                {
                    constexpr const char *getWageSql =
                        R"(SELECT wage
                           FROM jobsinfo
                           WHERE name = :jobName<char[18]>)";
                    otl_stream getWageStream(8, getWageSql, db);
                    getWageStream << jobName;

                    int wage;
                    for (auto &r : getWageStream)
                    {
                        r >> wage;
                    }

                    constexpr const char *updateTuotuoSql =
                        R"(UPDATE petprofile
                           SET tuotuo = tuotuo + :wage<int>
                           WHERE id = :id<char[16]>)";
                    otl_stream updateTuotuoStream(1, updateTuotuoSql, db);
                    updateTuotuoStream << wage << id;
                }

                reply = R"({"status":"succeeded"})";

#ifdef DEBUG
                std::cout << "In function Connection::DealWithWorkEnd: " << id
                          << " reply: " << reply;
#endif // DEBUG

                DoWrite();
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

void Connection::DealWithJobsInfo(const char *id, const char *randomKey, const std::vector<const char *> &jobs)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithJobsInfo: " << id << randomKey << std::endl;
#endif // DEBUG

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

        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始查询了
            {
                // 回复的json格式为：
                // {
                //   "jobs":
                //   [
                //     {
                //       "name": string,
                //       "wage": int,
                //       "lowestLevel": int,
                //       "eduRestrictions": string
                //     },
                //     ...
                //   ]
                //  }
                std::string replyJson = R"({"jobs":[)";
                replyJson.reserve(64);

                for (const auto &jobName : jobs)
                {
                    constexpr const char *selectItemInfoSqlStr =
                        R"(SELECT name, wage, lowestLevel, eduRestrictions FROM jobsinfo
                           WHERE name = :name<char[18]>)";
                    otl_stream selectItemInfoStream(196, selectItemInfoSqlStr, db);
                    selectItemInfoStream << jobName;

                    char name[18 + 1];
                    int wage, lowestLevel;
                    char eduRestrictions[128 + 1];
                    for (auto &in : selectItemInfoStream)
                    {
                        in >> name >> wage >> lowestLevel >> eduRestrictions;
                    }

                    replyJson += R"({"name":")";
                    replyJson += name;
                    replyJson += R"(","wage":)";
                    replyJson += std::to_string(wage);
                    replyJson += R"(","lowestLevel":)";
                    replyJson += std::to_string(lowestLevel);
                    replyJson += R"(","eduRestrictions":)";
                    replyJson += eduRestrictions;
                    replyJson += R"(},)";
                }

                // 如果数组不空，则最后一定存在一个','需要被去除
                if (replyJson.back() == ',')
                {
                    replyJson.pop_back();   // 去除最后一个','
                }
                replyJson += R"(]})";
                reply = std::move(replyJson);

#ifdef DEBUG
                std::cout << "In function Connection::DealWithJobsInfo: " << id
                          << " reply: " << reply << '\n';

#endif // DEBUG

                DoWrite();
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

void Connection::DealWithWorkStatus(const char *id, const char *randomKey)
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

        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始查询了
            {
                // 服务器返回的json格式
                // {
                //    "isWorking": bool,
                //    "job": string,    // 当isWorking为true时存在
                //    "time": int       // 当isWorking为true时存在
                // }

                constexpr const char *findStatusSql =
                    R"(SELECT working, job, TIMESTAMPDIFF(MINUTE, begintime, NOW())
                       FROM workinginfo
                       WHERE id = :id<char[16]>)";
                otl_stream findStatusStream(64, findStatusSql, db);
                findStatusStream << id;

                int isWorking;
                char job[18 + 1];
                int workingTime;
                bool ok = true, doubleIsWorking = false;
                for (auto &r : findStatusStream)
                {
                    r >> isWorking >> job >> workingTime;

                    if (isWorking && doubleIsWorking)
                    {
                        ok = false;

                        break;
                    }
                    else if (isWorking && !doubleIsWorking)
                    {
                        doubleIsWorking = true;
                    }
                }
                if (ok)
                {
                    if (isWorking)
                    {
                        reply = R"({"isWorking":true,"job":")" + std::string(job) + R"(","time":)" + std::to_string(workingTime) + "}";
                    }
                    else
                    {
                        reply = R"({"isWorking":false})";
                    }
                }
                else
                {
                    reply = R"("errmsg":"database error")";
                }

                DoWrite();
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

void Connection::DealWithFoodValue(const char *id, const char *randomKey)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithFoodValue: " << id << randomKey << std::endl;
#endif // DEBUG

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

        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始查询了
            {
                constexpr const char *selectFoodValueSql =
                        R"(SELECT food
                           FROM petprofile
                           WHERE id = :id<char[16]>)";
                otl_stream selectFoodValueStream(12, selectFoodValueSql, db);
                selectFoodValueStream << id;

                int foodValue;
                for (auto &r : selectFoodValueStream)
                {
                    r >> foodValue;
                }

                reply = R"({"food":)" + std::to_string(foodValue) + '}';
#ifdef DEBUG
                std::cout << "In function Connection::DealWithFoodValue: " << id
                          << " reply: " << reply;
#endif // DEBUG

                DoWrite();
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

void Connection::DealWithCleanValue(const char *id, const char *randomKey)
{
#ifdef DEBUG
    std::cout << "In function Connection::DealWithCleanValue: " << id << randomKey << std::endl;
#endif // DEBUG

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

        if (online) // 如果在线则检查randomKey是否一致，有点cookies的感觉
        {
            if (std::strncmp(randomKey, trueSecretKey, 18) == 0)    // 如果连randomKey都一致，那就可以开始查询了
            {
                constexpr const char *selectCleanValueSql =
                        R"(SELECT clean
                           FROM petprofile
                           WHERE id = :id<char[16]>)";
                otl_stream selectCleanValueStream(12, selectCleanValueSql, db);
                selectCleanValueStream << id;

                int cleanValue;
                for (auto &r : selectCleanValueStream)
                {
                    r >> cleanValue;
                }

                reply = R"({"clean":)" + std::to_string(cleanValue) + '}';
#ifdef DEBUG
                std::cout << "In function Connection::DealWithCleanValue: " << id
                          << " reply: " << reply;
#endif // DEBUG

                DoWrite();
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
