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
}

void CleanWindow::DataPrepare()
{
    QFile jsonFile("MianPetData/CleanShopData.json");
    if (!jsonFile.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        return;
    }

    const auto localJson = QJsonDocument::fromJson(jsonFile.readAll());
    itemNames = localJson["items"].toArray();

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
}

void CleanWindow::ViewNextPage()
{
}
