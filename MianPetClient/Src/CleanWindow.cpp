#include "CleanWindow.h"

CleanWindow::CleanWindow(QWidget *parent) : SmallItemsContainerWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

void CleanWindow::InitializeUi()
{
    std::thread thread(&CleanWindow::DataPrepare, this);
    thread.detach();

    SetWindowTitle("清洁");
}

void CleanWindow::InitializeConnect()
{
    connect(previousPageButton, &QPushButton::clicked, this, &CleanWindow::ViewPreviousPage);
    connect(nextPageButton, &QPushButton::clicked, this, &CleanWindow::ViewNextPage);
}

void CleanWindow::DataPrepare()
{
    QFile jsonFile("MianPetData/CleanShopData.json");
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

    tcpSocket->write(CleanShopRequestData{});
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

void CleanWindow::ViewPreviousPage()
{
    if (currentPage > 0)
    {
        --currentPage;
        const auto rangeBegin = currentPage * 4;
        const auto rangeEnd = rangeBegin + 4;

        item0->setIcon(QIcon(":/" + items[rangeBegin + 0].first));
        item1->setIcon(QIcon(":/" + items[rangeBegin + 1].first));
        item2->setIcon(QIcon(":/" + items[rangeBegin + 2].first));
        item3->setIcon(QIcon(":/" + items[rangeBegin + 3].first));
        // 展示[rangeBegin, rangeEnd)中的内容
    }
}

void CleanWindow::ViewNextPage()
{
    const auto max = static_cast<int>(items.size());
    if (max >= 2 && currentPage <= max - 2)
    {
        ++currentPage;
        const auto rangeBegin = currentPage * 4;
        const auto rangeEnd = std::min(rangeBegin + 4, max);

        item0->setIcon(QIcon(":/" + items[rangeBegin + 0].first));
        item1->setIcon(QIcon(":/" + items[rangeBegin + 1].first));
        item2->setIcon(QIcon(":/" + items[rangeBegin + 2].first));
        item3->setIcon(QIcon(":/" + items[rangeBegin + 3].first));
        // 展示[rangeBegin, rangeEnd)中的内容
    }
}
