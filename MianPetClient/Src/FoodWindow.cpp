#include "FoodWindow.h"

FoodWindow::FoodWindow(QWidget *parent) 
    : SmallItemsContainerWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

void FoodWindow::InitializeUi()
{
    std::thread thread(&FoodWindow::DataPrepare, this);
    thread.detach();

    SetIcon(FoodWindowIcon);
    SetWindowTitle("喂食");
}

void FoodWindow::InitializeConnect()
{
    connect(previousPageButton, &QPushButton::clicked, this, &FoodWindow::ViewPreviousPage);
    connect(nextPageButton, &QPushButton::clicked, this, &FoodWindow::ViewNextPage);
}

void FoodWindow::DataPrepare()
{
    QFile jsonFile("MianPetData/FoodShopData.json");
    if (!jsonFile.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        return;
    }

    const auto localJson = QJsonDocument::fromJson(jsonFile.readAll());
    const auto tempArray = localJson["items"].toArray();
    for (int i = 0; i < tempArray.size(); ++i)
    {
        const auto itemInfo = tempArray[i].toObject();
        items.emplace_back(itemInfo["name"].toString(), itemInfo["price"].toInt());
    }

    auto tcpSocket = std::make_unique<QTcpSocket>();
    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
    if (!tcpSocket->waitForConnected())
    {
        return;
    }

    tcpSocket->write(FoodShopRequestData{});
    if (!tcpSocket->waitForBytesWritten())
    {
        return;
    }

    if (!tcpSocket->waitForReadyRead())
    {
        return;
    }

    const auto remoteJson = QJsonDocument::fromJson(tcpSocket->readAll());
    // 处理remoteJson
}

void FoodWindow::ViewPreviousPage()
{
    if (currentPage > 0)
    {
        --currentPage;
        const auto rangeBegin = currentPage * 4;
        const auto rangeEnd = rangeBegin + 4;

        // 展示[rangeBegin, rangeEnd)中的内容
    }
}

void FoodWindow::ViewNextPage()
{
    const auto max = static_cast<int>(items.size());
    if (max >= 2 && currentPage <= max - 2)
    {
        ++currentPage;
        const auto rangeBegin = currentPage * 3;
        const auto rangeEnd = std::min(rangeBegin + 4, max);

        // 展示[rangeBegin, rangeEnd)中的内容
    }
}
