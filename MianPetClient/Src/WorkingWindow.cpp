#include "WorkingWindow.h"

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
        //--currentPage;
        //const auto rangeBegin = currentPage * 4;
        //const auto rangeEnd = rangeBegin + 4;

        //std::thread requestThread(&FoodWindow::RequestDataInRange, this, rangeBegin, rangeEnd);
        //requestThread.detach();

        //item0->setIcon(QIcon(":/Pic/" + items[rangeBegin + 0].name + ".png"));
        //item1->setIcon(QIcon(":/Pic/" + items[rangeBegin + 1].name + ".png"));
        //item2->setIcon(QIcon(":/Pic/" + items[rangeBegin + 2].name + ".png"));
        //item3->setIcon(QIcon(":/Pic/" + items[rangeBegin + 3].name + ".png"));
        //// 展示[rangeBegin, rangeEnd)中的内容

        //selectedJob.clear();
    }
}

void WorkingWindow::ViewNextPage()
{
}

void WorkingWindow::OnMouseHoversMoveItem(QObject *obj)
{
}

void WorkingWindow::OnMouseHoversLeaveItem(QObject *obj)
{
}
