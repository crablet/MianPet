#include "CleanWindow.h"

CleanWindow::CleanWindow(QWidget *parent) : SmallItemsContainerWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

CleanWindow::~CleanWindow()
{
    // 退出的时候保存一下缓存

    QJsonArray arr;
    for (const auto &r : items) // 遍历items中的内容，然后将其组成一个大的json数组
    {
        QJsonObject obj;
        obj.insert("amount", r.amount);
        obj.insert("name", r.name);
        obj.insert("price", r.price);

        arr.push_back(obj);
    }

    QFile jsonFile("MianPetData/CleanShopData.json");
    if (!jsonFile.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))   // 打开要存放的缓存文件
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
    buyButton->setDisabled(true);
    useButton = new QPushButton("使用", this);
    useButton->setDisabled(true);

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

    // 点击第一个按钮
    connect(item0, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 0;
            index < items.size())   // 确保index不要越界，因为有可能有按钮没有对应的清洁品
        {
            selectedClean = items[index].name;

            buyButton->setEnabled(true);
            useButton->setEnabled(true);
        }
        else
        {
            selectedClean.clear();

            buyButton->setDisabled(true);
            useButton->setDisabled(true);
        }
    });

    // 点击第二个按钮
    connect(item1, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 1;
            index < items.size())   // 确保index不要越界，因为有可能有按钮没有对应的清洁品
        {
            selectedClean = items[index].name;

            buyButton->setEnabled(true);
            useButton->setEnabled(true);
        }
        else
        {
            selectedClean.clear();

            buyButton->setDisabled(true);
            useButton->setDisabled(true);
        }
    });

    // 点击第三个按钮
    connect(item2, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 2;
            index < items.size())   // 确保index不要越界，因为有可能有按钮没有对应的清洁品
        {
            selectedClean = items[index].name;

            buyButton->setEnabled(true);
            useButton->setEnabled(true);
        }
        else
        {
            selectedClean.clear();

            buyButton->setDisabled(true);
            useButton->setDisabled(true);
        }
    });

    // 点击第四个按钮
    connect(item3, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 3;
            index < items.size())   // 确保index不要越界，因为有可能有按钮没有对应的清洁品
        {
            selectedClean = items[index].name;

            buyButton->setEnabled(true);
            useButton->setEnabled(true);
        }
        else
        {
            selectedClean.clear();

            buyButton->setDisabled(true);
            useButton->setDisabled(true);
        }
    });

    connect(buyButton, &QPushButton::clicked, this, &CleanWindow::OnBuyButtonClicked);
    connect(useButton, &QPushButton::clicked, this, &CleanWindow::OnUseButtonClicked);

    connect(this, &CleanWindow::BuySucceeded, this, [=](const QString &item, int count)
    {
        QMessageBox::information(this, "购买成功", "本次共购入" + QString::number(count) + "个" + item);
    });
    connect(this, &CleanWindow::BuyFailed, this, [=]()
    {
        QMessageBox::information(this, "购买失败", "请检查坨坨余额是否充足");
    });

    connect(this, &CleanWindow::UseSucceeded, this, [=](const QString &item, int count)
    {
        QMessageBox::information(this, "使用成功", "本次共使用" + QString::number(count) + "个" + item);
    });
    connect(this, &CleanWindow::UseFailed, this, [=]()
    {
        QMessageBox::information(this, "使用失败", "我们真的有这个吗");
    });
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
    for (int i = 0; i < tempArray.size(); ++i)  // 将本地缓存(MianPetData/CleanShopData.json)先存到items中
    {
        const auto itemInfo = tempArray[i].toObject();
        items.emplace_back(itemInfo["name"].toString(), itemInfo["price"].toInt(), itemInfo["amount"].toInt());
    }

    // 要保证展示的图标在范围内，不要越界了
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
    if (currentPage > 0)    // 只有页码大于零的情况下才可以翻上一页，不然一定越界了
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

        selectedClean.clear();  // 翻页后选中的清洁品要清空，不然就会被上一页的遗留信息影响了

        // 既然可以翻页，那么下方两个按钮都可以正常展示
        buyButton->setDisabled(true);   
        useButton->setDisabled(true);
    }
}

void CleanWindow::ViewNextPage()
{
    const auto max = static_cast<int>(items.size());
    if (max >= 2 && currentPage <= max - 2) // 只有页码小于max - 2才可以翻下一页，不然一定越界了
    {
        ++currentPage;
        const auto rangeBegin = currentPage * 4;
        const auto rangeEnd = std::min(rangeBegin + 4, max);

        // 请求当前页面所需要的数据
        std::thread requestThread(&CleanWindow::RequestDataInRange, this, rangeBegin, rangeEnd);
        requestThread.detach(); // 开启请求线程后就可以扔一边不管了，让它自己去更新数据

        // 展示[rangeBegin, rangeEnd)中的内容
        rangeBegin + 0 < rangeEnd       // 确保不要越界
            ? item0->setIcon(QIcon(":/Pic/" + items[rangeBegin + 0].name + ".png"))
            : item0->setIcon(QIcon());  // 如果越界了就展示空图标
        rangeBegin + 1 < rangeEnd       // 确保不要越界
            ? item1->setIcon(QIcon(":/Pic/" + items[rangeBegin + 1].name + ".png"))
            : item1->setIcon(QIcon());  // 如果越界了就展示空图标
        rangeBegin + 2 < rangeEnd       // 确保不要越界
            ? item2->setIcon(QIcon(":/Pic/" + items[rangeBegin + 2].name + ".png"))
            : item2->setIcon(QIcon());  // 如果越界了就展示空图标
        rangeBegin + 3 < rangeEnd       // 确保不要越界
            ? item3->setIcon(QIcon(":/Pic/" + items[rangeBegin + 3].name + ".png"))
            : item3->setIcon(QIcon());  // 如果越界了就展示空图标

        selectedClean.clear();  // 翻页后选中的清洁品要清空，不然就会被上一页的遗留信息影响了

        // 既然可以翻页，那么下方两个按钮都可以正常展示
        buyButton->setDisabled(true);   
        useButton->setDisabled(true);
    }
}

void CleanWindow::OnMouseHoversMoveItem(QObject *obj)
{
    // 判断鼠标放在了哪个控件上面，然后根据控件编号设置展示的itemLabel中的内容应写什么
    if (obj == item0 && currentPage * 4 + 0 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 0].name);
        itemLabel->SetLowerLabelText(QString::number(items[currentPage * 4 + 0].amount));
    }
    else if (obj == item1 && currentPage * 4 + 1 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 1].name);
        itemLabel->SetLowerLabelText(QString::number(items[currentPage * 4 + 1].amount));
    }
    else if (obj == item2 && currentPage * 4 + 2 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 2].name);
        itemLabel->SetLowerLabelText(QString::number(items[currentPage * 4 + 2].amount));
    }
    else if (obj == item3 && currentPage * 4 + 3 < items.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(items[currentPage * 4 + 3].name);
        itemLabel->SetLowerLabelText(QString::number(items[currentPage * 4 + 3].amount));
    }
    else
    {
        // 鼠标放在的地方没有控件，应该收起itemLabel并且清空其中内容，以免下次展示时出现闪烁
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
        data.SetCount(1);   // TODO: 请求的物品数目，目前只支持每次都请求一个，以后再实现一次请求多个
        tcpSocket->write(data);
        if (!tcpSocket->waitForBytesWritten())
        {
            return;
        }

        if (!tcpSocket->waitForReadyRead())
        {
            return;
        }

        const auto remoteJson = QJsonDocument::fromJson(tcpSocket->readAll());
        const auto status = remoteJson["status"].toString();
        if (status == "succeeded")
        {
            emit BuySucceeded(selectedClean, 1);
            // 购买成功
        }
        else if (status == "failed")
        {
            emit BuyFailed(selectedClean, 1);
            // 购买失败
        }
        else
        {
            // error
        }
    });
    thread.detach();
}

void CleanWindow::OnUseButtonClicked()
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

        UseRequestData data;
        data.SetItem(selectedClean);
        data.SetCount(1);   // TODO: 请求的物品数目，目前只支持每次都请求一个，以后再实现一次请求多个
        tcpSocket->write(data);
        if (!tcpSocket->waitForBytesWritten())
        {
            return;
        }

        if (!tcpSocket->waitForReadyRead())
        {
            return;
        }

        const auto remoteJson = QJsonDocument::fromJson(tcpSocket->readAll());
        const auto status = remoteJson["status"].toString();
        if (status == "succeeded")
        {
            emit UseSucceeded(selectedClean, 1);
            // 使用成功
        }
        else if (status == "failed")
        {
            emit UseFailed(selectedClean, 1);
            // 使用失败
        }
        else
        {
            // error
        }
    });
    thread.detach();
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
    const auto jsonArray = remoteJson["items"].toArray();
    for (const auto &r : jsonArray)
    {
        const auto obj = r.toObject();
        const auto name = r["name"].toString();
        const auto amount = r["amount"].toInt();

        auto iter = std::find_if(items.begin(), items.end(), [=, &name](const ItemInformation &rhs)
        {
            return name == rhs.name;
        });
        iter->amount = amount;  // iter可能为空
    }
}
