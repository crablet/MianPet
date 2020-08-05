﻿#include "WorkingWindow.h"

WorkingWindow::WorkingWindow()
{
    InitializeUi();
    InitializeConnect();
}

void WorkingWindow::InitializeUi()
{
    SetWindowTitle("打工");
}

void WorkingWindow::InitializeConnect()
{
}

void WorkingWindow::ViewPreviousPage()
{
    if (currentPage > 0)
    {
        --currentPage;
        const auto rangeBegin = currentPage * 4;
        const auto rangeEnd = rangeBegin + 4;

        std::thread requestThread(&WorkingWindow::RequestDataInRange, this, rangeBegin, rangeEnd);
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

        std::thread requestThread(&WorkingWindow::RequestDataInRange, this, rangeBegin, rangeEnd);
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

void WorkingWindow::RequestDataInRange(int rangeBegin, int rangeEnd)
{
}