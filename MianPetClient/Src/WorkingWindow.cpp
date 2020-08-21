#include "WorkingWindow.h"

WorkingWindow::WorkingWindow(QWidget *parent)
    : SmallItemsContainerWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

// 退出时保存最新的职位信息
WorkingWindow::~WorkingWindow()
{
    QJsonArray arr;
    for (const auto &r : jobs)
    {
        QJsonArray eduRestrictions;
        for (const auto &edu : r.eduRestrictions)
        {
            eduRestrictions.push_back(edu);
        }

        QJsonObject obj;
        obj.insert("name", r.name);
        obj.insert("wage", r.wage);
        obj.insert("lowestLevel", r.lowestLevel);
        obj.insert("eduRestrictions", eduRestrictions);

        arr.push_back(obj);
    }

    QFile jsonFile("MianPetData/JobsData.json");
    if (!jsonFile.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    else
    {
        QJsonObject obj;
        obj.insert("jobs", arr);
        jsonFile.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    }
}

void WorkingWindow::InitializeUi()
{
    GetWorkingStatus(); // 我们有理由相信，与服务器沟通的时间要比初始化下面的控件时间要长，所以先执行此操作

    SetWindowTitle("打工");

    beginButton = new QPushButton("开始", this);
    beginButton->setDisabled(true);
    endButton = new QPushButton("结束", this);
    endButton->setDisabled(true);

    lowerBarWidgetLayout = new QHBoxLayout(this);
    lowerBarWidgetLayout->addStretch();
    lowerBarWidgetLayout->addWidget(beginButton);
    lowerBarWidgetLayout->addWidget(endButton);
    lowerBarWidgetLayout->addStretch();

    lowerBarWidget = new QWidget(this);
    lowerBarWidget->setLayout(lowerBarWidgetLayout);
    lowerBarWidget->setFixedSize(WorkingWindowLowerBarWidgetWidth, WorkingWindowLowerBarWidgetHeight);
    lowerBarWidget->move(WorkingWindowLowerBarWidgetX, WorkingWindowLowerBarWidgetY);
}

void WorkingWindow::InitializeConnect()
{
    connect(this, &WorkingWindow::WorkBeginSucceeded, this, [=]()
    {
        QMessageBox::information(this, "成功", "开始打工！");
    });
    connect(this, &WorkingWindow::WorkBeginFailed, this, [=]()
    {
        QMessageBox::information(this, "失败", "我们好像打不了这个工。");
    });
    connect(this, &WorkingWindow::WorkEndSucceeded, this, [=]()
    {
        QMessageBox::information(this, "成功", "我们先不打这个工了。");
    });
    connect(this, &WorkingWindow::WorkEndFailed, this, [=]()
    {
        QMessageBox::information(this, "失败", "怎么办？这工作结束不了。");
    });

    connect(item0, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 0;
            index < jobs.size())
        {
            selectedJob = jobs[index].name;

            beginButton->setEnabled(true);
            endButton->setEnabled(true);
        }
        else
        {
            selectedJob.clear();

            beginButton->setDisabled(true);
            endButton->setDisabled(true);
        }
    });
    connect(item1, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 1;
            index < jobs.size())
        {
            selectedJob = jobs[index].name;

            beginButton->setEnabled(true);
            endButton->setEnabled(true);
        }
        else
        {
            selectedJob.clear();

            beginButton->setDisabled(true);
            endButton->setDisabled(true);
        }
    });
    connect(item2, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 2;
            index < jobs.size())
        {
            selectedJob = jobs[index].name;

            beginButton->setEnabled(true);
            endButton->setEnabled(true);
        }
        else
        {
            selectedJob.clear();

            beginButton->setDisabled(true);
            endButton->setDisabled(true);
        }
    });
    connect(item3, &QPushButton::clicked, this, [=]()
    {
        if (const auto index = currentPage * 4 + 3;
            index < jobs.size())
        {
            selectedJob = jobs[index].name;

            beginButton->setEnabled(true);
            endButton->setEnabled(true);
        }
        else
        {
            selectedJob.clear();

            beginButton->setDisabled(true);
            endButton->setDisabled(true);
        }
    });
}

void WorkingWindow::ViewPreviousPage()
{
    if (currentPage > 0)
    {
        --currentPage;
        const auto rangeBegin = currentPage * 4;
        const auto rangeEnd = rangeBegin + 4;

        std::thread requestThread(&WorkingWindow::RequestJobsInfoInRange, this, rangeBegin, rangeEnd);
        requestThread.detach();

        item0->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 0].name + ".png"));
        item1->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 1].name + ".png"));
        item2->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 2].name + ".png"));
        item3->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 3].name + ".png"));
        // 展示[rangeBegin, rangeEnd)中的内容

        selectedJob.clear();
    }
}

void WorkingWindow::ViewNextPage()
{
    const auto max = static_cast<int>(jobs.size());
    if (max >= 2 && currentPage <= max - 2)
    {
        ++currentPage;
        const auto rangeBegin = currentPage * 3;
        const auto rangeEnd = std::min(rangeBegin + 4, max);

        std::thread requestThread(&WorkingWindow::RequestJobsInfoInRange, this, rangeBegin, rangeEnd);
        requestThread.detach();

        rangeBegin + 0 < rangeEnd
            ? item0->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 0].name + ".png"))
            : item0->setIcon(QIcon());
        rangeBegin + 1 < rangeEnd
            ? item1->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 1].name + ".png"))
            : item1->setIcon(QIcon());
        rangeBegin + 2 < rangeEnd
            ? item2->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 2].name + ".png"))
            : item2->setIcon(QIcon());
        rangeBegin + 3 < rangeEnd
            ? item3->setIcon(QIcon(":/Pic/" + jobs[rangeBegin + 3].name + ".png"))
            : item3->setIcon(QIcon());
        // 展示[rangeBegin, rangeEnd)中的内容

        selectedJob.clear();
    }
}

void WorkingWindow::OnMouseHoversMoveItem(QObject *obj)
{
    // 判断鼠标放在了哪个控件上面，然后根据控件编号设置展示的itemLabel中的内容应写什么
    if (obj == item0 && currentPage * 4 + 0 < jobs.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(jobs[currentPage * 4 + 0].name);
        itemLabel->SetLowerLabelText(QString::number(jobs[currentPage * 4 + 0].wage));
    }
    else if (obj == item1 && currentPage * 4 + 1 < jobs.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(jobs[currentPage * 4 + 1].name);
        itemLabel->SetLowerLabelText(QString::number(jobs[currentPage * 4 + 1].wage));
    }
    else if (obj == item2 && currentPage * 4 + 2 < jobs.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(jobs[currentPage * 4 + 2].name);
        itemLabel->SetLowerLabelText(QString::number(jobs[currentPage * 4 + 2].wage));
    }
    else if (obj == item3 && currentPage * 4 + 3 < jobs.size())
    {
        itemLabel->show();
        itemLabel->SetUpperLabelText(jobs[currentPage * 4 + 3].name);
        itemLabel->SetLowerLabelText(QString::number(jobs[currentPage * 4 + 3].wage));
    }
    else
    {
        // 鼠标放在的地方没有控件，应该收起itemLabel并且清空其中内容，以免下次展示时出现闪烁
        itemLabel->hide();
        itemLabel->SetUpperLabelText("");
        itemLabel->SetLowerLabelText("");
    }
}

void WorkingWindow::OnMouseHoversLeaveItem([[maybe_unused]] QObject *obj)
{
    itemLabel->hide();
    itemLabel->SetUpperLabelText("");
    itemLabel->SetLowerLabelText("");
}

void WorkingWindow::RequestJobsInfoInRange(int rangeBegin, int rangeEnd)
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
        tempArray.append(jobs[i].name);
    }
    JobsInfoRequestData requestData;
    requestData.SetJobs(tempArray);
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
    const auto jsonArray = remoteJson["jobs"].toArray();
    for (const auto &r : jsonArray)
    {
        const auto obj = r.toObject();
        const auto name = r["name"].toString();
        const auto wage = r["wage"].toInt();
        const auto lowestLevel = r["lowestLevel"].toInt();
        const auto eduRestrictions = r["eduRestrictions"].toArray();

        std::vector<QString> eduRestrictionsVec;
        for (const auto &r : eduRestrictions)
        {
            eduRestrictionsVec.push_back(r.toString());
        }

        auto iter = std::find_if(jobs.begin(), jobs.end(), [=, &name](const JobInformation &rhs)
        {
            return name == rhs.name;
        });
        iter->name = name;  // iter可能为空
        iter->wage = wage;
        iter->lowestLevel = lowestLevel;
        iter->eduRestrictions = std::move(eduRestrictionsVec);    // 可以std::move吗？
    }
}

void WorkingWindow::SubmitWorkBeginRequest(const QString &jobName)
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

        WorkBeginRequest data;
        data.SetJob(jobName);
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
            emit WorkBeginSucceeded();
            // 开始打工成功
        }
        else if (status == "failed")
        {
            emit WorkBeginFailed();
            // 开始打工失败
        }
        else
        {
            // error
        }
    });
    thread.detach();
}

void WorkingWindow::SubmitWorkEndRequest(const QString &jobName)
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

        WorkEndRequest data;
        data.SetJob(jobName);
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
            emit WorkEndSucceeded();
            // 结束打工成功
        }
        else if (status == "failed")
        {
            emit WorkEndFailed();
            // 结束打工失败
        }
        else
        {
            // error
        }
    });
    thread.detach();
}

void WorkingWindow::GetWorkingStatus()
{
    std::thread thread(
    [this]()
    {
        auto tcpSocket = std::make_unique<QTcpSocket>();
        tcpSocket->connectToHost(ServerAddress, ServerPort, QTcpSocket::ReadWrite);
        if (!tcpSocket->waitForConnected())
        {
            return;
        }

        tcpSocket->write(WorkStatusRequestData{});
        if (!tcpSocket->waitForBytesWritten())
        {
            return;
        }

        if (!tcpSocket->waitForReadyRead())
        {
            return;
        }

        // 服务器返回的json格式
        // {
        //    "isWorking": bool,
        //    "job": string,    // 当isWorking为true时存在
        //    "time": int       // 当isWorking为true时存在
        // }
        const auto remoteJson = QJsonDocument::fromJson(tcpSocket->readAll());
        isWorking = remoteJson["isWorking"].toBool();
        if (isWorking)
        {
            workingJob = remoteJson["job"].toString();
            workingTime = remoteJson["time"].toInt();

            SetWindowTitle("打工-正在打工-" + workingJob + "已进行-" + QString::number(workingTime));
        }
        else
        {
            workingJob.clear();
            workingTime = 0;

            SetWindowTitle("打工");
        }
    });
    thread.detach();
}
