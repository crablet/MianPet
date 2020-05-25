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

                }
                else if (hint == CORE_KEY_FOR_PASSWORD_TRANSPORTATION)
                {
                    DealWithGetCoreKeyForPasswordTransportation();
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

void Connection::DealWithGetCoreKeyForPasswordTransportation()
{
    std::random_device rd;
    std::minstd_rand gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    reply = R"({"corekey":")" + std::to_string(dis(gen)) + R"("})";

    DoWrite();
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
        otl_stream otlCur(64, sqlStr, db);
        otlCur << id;

        char truePassword[32 + 1];
        int online;
        char trueSecretKey[18 + 1];
        for (auto &in : otlCur)
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
