#include "Connection.h"
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

#ifdef DEBUG
            std::cout << id << ' ' << randomKey << ' ' << version << ' ' << method << std::endl;
#endif // DEBUG

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

#ifdef DEBUG
        std::cout << "In DealWithGetLogin()" << std::endl;
        std::cout << id << ' ' << truePassword << ' '  << online << ' ' << trueSecretKey << std::endl;
#endif // DEBUG


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
                int status;
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
                    R"({"id":"%s","username":"%s","level":%d,"age":%d,"growth":%d,"food":%d,"clean":%d,"health":%d,"mood":%d,"growth_speed":%d,"status":"%d","online_time":%d})",
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
                int status;
                int onlineTime;
                for (auto &r : petprofileStream)
                {
                    r >> level >> age >> growth
                      >> food >> clean >> health >> mood >> growthSpeed
                      >> status >> onlineTime;
                }

                const auto newAge = age + minuteDelta;
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
                const auto newStatus = status;
                // TODO: finish the calculation

                const auto newOnlineTime = onlineTime + minuteDelta;

                constexpr const char *updateProfileSql =
                    R"(UPDATE petprofile
                       SET level = :newLevel<int>, age = :newAge<int>, growth = :newGrowth<int>, 
                           food = :newFood<int>, clean = :newClean<int>, health = :newHealth<int>, mood = :newMood<int>, growth_speed = :newGrowthSpeed<int>, 
                           status = :newStatus<int>, online_time = :newOnlineTime<int>
                       WHERE id = :id<char[16]>)";
                otl_stream updateProfileStream(1, updateProfileSql, db);
                updateProfileStream << newLevel << newAge << newGrowth 
                                    << newFood << newClean << newHealth << newMood << newGrowthSpeed
                                    << newStatus << newOnlineTime
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

void Connection::DealWithFoodShopInfo(const char *id, const char *randomKey, const std::vector<const char*> &items)
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
                //       "name": string
                //       "amount": int
                //     },
                //     ...
                //   ]
                //  }
                std::string replyJson = R"({"items":[)";

                for (const auto &itemname : items)
                {
                    constexpr const char *selectItemInfoSqlStr =
                        R"(SELECT quantity FROM ownitems
                           WHERE id = :id<char[16]> AND itemname = :itemname<char[18]>)";
                    otl_stream selectItemInfoStream(64, selectItemInfoSqlStr, db);
                    selectItemInfoStream << id << itemname;

                    int amount;
                    for (auto &in : selectItemInfoStream)
                    {
                        in >> amount;
                    }

                    replyJson += R"({"name":")";
                    replyJson += itemname;
                    replyJson += R"(","amount":)";
                    replyJson += std::to_string(amount);
                    replyJson += R"(},)";
                }

                replyJson.pop_back();   // 去除最后一个','
                replyJson += R"(]})";
                reply = std::move(replyJson);

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
                //       "name": string
                //       "amount": int
                //     },
                //     ...
                //   ]
                //  }
                std::string replyJson = R"({"items":[)";

                for (const auto &itemname : items)
                {
                    constexpr const char *selectItemInfoSqlStr =
                        R"(SELECT quantity FROM ownitems
                           WHERE id = :id<char[16]> AND itemname = :itemname<char[18]>)";
                    otl_stream selectItemInfoStream(64, selectItemInfoSqlStr, db);
                    selectItemInfoStream << id << itemname;

                    int amount;
                    for (auto &in : selectItemInfoStream)
                    {
                        in >> amount;
                    }

                    replyJson += R"({"name":")";
                    replyJson += itemname;
                    replyJson += R"(","amount":)";
                    replyJson += std::to_string(amount);
                    replyJson += R"(},)";
                }

                replyJson.pop_back();   // 去除最后一个','
                replyJson += R"(]})";
                reply = std::move(replyJson);

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

                    // 计算使用后的属性值，最大值为3200
                    food = std::min(3200, food + foodBonus);
                    clean = std::min(3200, clean + cleanBonus);
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
}

void Connection::DealWithWorkEnd(const char *id, const char *randomKey, const char *job)
{
}

void Connection::DealWithJobsInfo(const char *id, const char *randomKey, const std::vector<const char *> &jobs)
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

                replyJson.pop_back();   // 去除最后一个','
                replyJson += R"(]})";
                reply = std::move(replyJson);

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

                reply = "";

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
