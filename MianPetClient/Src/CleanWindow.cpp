﻿#include "CleanWindow.h"

CleanWindow::CleanWindow(QWidget *parent) : SmallItemsContainerWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

CleanWindow::~CleanWindow()
{
    QJsonArray arr;
    for (const auto &r : items)
    {
        QJsonObject obj;
        obj.insert("amount", r.amount);
        obj.insert("name", r.name);
        obj.insert("price", r.price);

        arr.push_back(obj);
    }

    QFile jsonFile("MianPetData/CleanShopData.json");
    if (!jsonFile.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    else
    {
        QJsonObject obj;
        obj.insert("items", arr);
        jsonFile.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    }
}

void CleanWindow::InitializeUi()
{
    std::thread thread(&CleanWindow::DataPrepare, this);
    thread.detach();

    SetWindowTitle("清洁");

    lowerBarTextLabel = new QLabel("清洁值", this);
    valueBar = new QProgressBar(this);
    buyButton = new QPushButton("购买", this);
    useButton = new QPushButton("使用", this);

    valueBar->setAlignment(Qt::AlignCenter);
    valueBar->setValue(cleanValue);

    lowerBarWidgetLayout = new QHBoxLayout(this);
    lowerBarWidgetLayout->addWidget(lowerBarTextLabel);
    lowerBarWidgetLayout->addWidget(valueBar);
    lowerBarWidgetLayout->addWidget(buyButton);
    lowerBarWidgetLayout->addWidget(useButton);

    lowerBarWidget = new QWidget(this);
    lowerBarWidget->setLayout(lowerBarWidgetLayout);
    lowerBarWidget->setFixedSize(CleanWindowLowerBarWidgetWidth, CleanWindowLowerBarWidgetHeight);
    lowerBarWidget->move(CleanWindowLowerBarWidgetX, CleanWindowLowerBarWidgetY);
}

void CleanWindow::InitializeConnect()
{
    connect(previousPageButton, &QPushButton::clicked, this, &CleanWindow::ViewPreviousPage);
    connect(nextPageButton, &QPushButton::clicked, this, &CleanWindow::ViewNextPage);

    connect(this, &CleanWindow::MouseHoversMoveOnItem, this, &CleanWindow::OnMouseHoversMoveItem);
    connect(this, &CleanWindow::MouseHoversLeaveOnItem, this, &CleanWindow::OnMouseHoversLeaveItem);

    connect(item0, &QPushButton::clicked, this, [=]()
    {
        selectedClean = items[currentPage * 4 + 0].name;
    });
    connect(item1, &QPushButton::clicked, this, [=]()
    {
        selectedClean = items[currentPage * 4 + 1].name;
    });
    connect(item2, &QPushButton::clicked, this, [=]()
    {
        selectedClean = items[currentPage * 4 + 2].name;
    });
    connect(item3, &QPushButton::clicked, this, [=]()
    {
        selectedClean = items[currentPage * 4 + 3].name;
    });

    connect(buyButton, &QPushButton::clicked, this, &CleanWindow::OnBuyButtonClicked);
    connect(useButton, &QPushButton::clicked, this, &CleanWindow::OnUseButtonClicked);
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
        items.emplace_back(itemInfo["name"].toString(), itemInfo["price"].toInt(), itemInfo["amount"].toInt());
    }

    const auto rangeBegin = 0;
    const auto rangeEnd = static_cast<decltype(rangeBegin)>(items.size() >= 4 ? 4 : items.size());
    rangeBegin + 0 < rangeEnd
        ? item0->setIcon(QIcon(":/Pic/" + items[rangeBegin + 0].name + ".png"))
        : item0->setIcon(QIcon());
    rangeBegin + 1 < rangeEnd
        ? item1->setIcon(QIcon(":/Pic/" + items[rangeBegin + 1].name + ".png"))
        : item1->setIcon(QIcon());
    rangeBegin + 2 < rangeEnd
        ? item2->setIcon(QIcon(":/Pic/" + items[rangeBegin + 2].name + ".png"))
        : item2->setIcon(QIcon());
    rangeBegin + 3 < rangeEnd
        ? item3->setIcon(QIcon(":/Pic/" + items[rangeBegin + 3].name + ".png"))
        : item3->setIcon(QIcon());

    currentPage = 0;

    RequestDataInRange(rangeBegin, rangeEnd);
}

void CleanWindow::ViewPreviousPage()
{
    if (currentPage > 0)
    {
        --currentPage;
        const auto rangeBegin = currentPage * 4;
        const auto rangeEnd = rangeBegin + 4;

        std::thread requestThread(&CleanWindow::RequestDataInRange, this, rangeBegin, rangeEnd);
        requestThread.detach();

        item0->setIcon(QIcon(":/Pic/" + items[rangeBegin + 0].name + ".png"));
        item1->setIcon(QIcon(":/Pic/" + items[rangeBegin + 1].name + ".png"));
        item2->setIcon(QIcon(":/Pic/" + items[rangeBegin + 2].name + ".png"));
        item3->setIcon(QIcon(":/Pic/" + items[rangeBegin + 3].name + ".png"));
        // 展示[rangeBegin, rangeEnd)中的内容

        selectedClean.clear();
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

        std::thread requestThread(&CleanWindow::RequestDataInRange, this, rangeBegin, rangeEnd);
        requestThread.detach();

        rangeBegin + 0 < rangeEnd
            ? item0->setIcon(QIcon(":/Pic/" + items[rangeBegin + 0].name + ".png"))
            : item0->setIcon(QIcon());
        rangeBegin + 1 < rangeEnd
            ? item1->setIcon(QIcon(":/Pic/" + items[rangeBegin + 1].name + ".png"))
            : item1->setIcon(QIcon());
        rangeBegin + 2 < rangeEnd
            ? item2->setIcon(QIcon(":/Pic/" + items[rangeBegin + 2].name + ".png"))
            : item2->setIcon(QIcon());
        rangeBegin + 3 < rangeEnd
            ? item3->setIcon(QIcon(":/Pic/" + items[rangeBegin + 3].name + ".png"))
            : item3->setIcon(QIcon());
        // 展示[rangeBegin, rangeEnd)中的内容

        selectedClean.clear();
    }
}

void CleanWindow::OnMouseHoversMoveItem(QObject *obj)
{
    if (obj == item0 && currentPage * 4 + 0 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 0].name);
    }
    else if (obj == item1 && currentPage * 4 + 1 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 1].name);
    }
    else if (obj == item2 && currentPage * 4 + 2 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 2].name);
    }
    else if (obj == item3 && currentPage * 4 + 3 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 3].name);
    }
    else
    {
        itemLabel->hide();
        itemLabel->SetUpperLabelText("");
        itemLabel->SetLowerLabelText("");
    }
}

void CleanWindow::OnMouseHoversLeaveItem([[maybe_unused]] QObject *obj)
{
    itemLabel->hide();
    itemLabel->SetUpperLabelText("");
    itemLabel->SetLowerLabelText("");
}

void CleanWindow::OnBuyButtonClicked()
{
    std::thread thread(
    [=]()
    {
        auto tcpSocket = std::make_unique<QTcpSocket>();
        tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
        if (!tcpSocket->waitForConnected())
        {
            return;
        }

        BuyRequestData data;
        data.SetItem(selectedClean);
        data.SetCount(1);
        tcpSocket->write(data);
        if (!tcpSocket->waitForBytesWritten())
        {
            return;
        }

        if (!tcpSocket->waitForReadyRead())
        {
            return;
        }

        const auto message = tcpSocket->readAll();
        // 处理服务器的返回信息，购买成功/失败要做相应的处理
    });
    thread.detach();
}

void CleanWindow::OnUseButtonClicked()
{
    qDebug() << "Use " << selectedClean;
}

void CleanWindow::RequestDataInRange(int rangeBegin, int rangeEnd)
{
    auto tcpSocket = std::make_unique<QTcpSocket>();
    tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
    if (!tcpSocket->waitForConnected())
    {
        return;
    }

    QJsonArray tempArray;
    for (int i = rangeBegin; i < rangeEnd; ++i)
    {
        tempArray.append(items[i].name);
    }
    CleanShopRequestData requestData;
    requestData.SetItems(tempArray);
    tcpSocket->write(requestData);
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
    // 并把更新写入CleanShopData.json和items中
}
