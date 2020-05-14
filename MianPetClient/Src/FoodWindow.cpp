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
}

void FoodWindow::DataPrepare()
{
    QFile jsonFile("MianPetData/FoodShopData.json");
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
}

void FoodWindow::ViewNextPage()
{
}
