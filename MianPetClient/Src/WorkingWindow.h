#ifndef WORKINGWINDOW_H
#define WORKINGWINDOW_H

#include <utility>
#include <vector>
#include <thread>
#include <algorithm>
#include <memory>

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QTcpSocket>
#include <QMessageBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

#include "SmallItemsContainerWindow.h"
#include "PetStatusData.h"

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/WorkBeginRequest.h"
#include "Tools/Network/DataPackages/WorkEndRequest.h"
#include "Tools/Network/DataPackages/JobsInfoRequestData.h"
#include "Tools/Network/DataPackages/WorkStatusRequestData.h"

class WorkingWindow final : public SmallItemsContainerWindow
{
    Q_OBJECT
public:
    explicit WorkingWindow(QWidget *parent = nullptr);
    ~WorkingWindow();

private:
    void InitializeUi();
    void InitializeConnect();

    void ViewPreviousPage();
    void ViewNextPage();

    void OnMouseHoversMoveItem(QObject *obj);
    void OnMouseHoversLeaveItem(QObject *obj);

    void OnBeginButtonClicked();
    void OnEndButtonClicked();

    // 根据jobs中的下标范围向服务器请求职位具体数据
    void RequestJobsInfoInRange(int rangeBegin, int rangeEnd);
    // 向服务器提交开始打jobName工的请求
    void SubmitWorkBeginRequest(const QString &jobName);
    // 向服务器提交结束打jobName工的请求
    void SubmitWorkEndRequest(const QString &jobName);

    // 获取当前打工状态，并作出相应响应（是否应该在此处做出响应，还是应该交给别的函数处理？）
    void GetWorkingStatus();

    // 获取最新的职位信息
    void DataPrepare();

    // 存放职位信息的结构体
    struct JobInformation
    {
        JobInformation(QString name, int wage, int lowestLevel, std::vector<QString> eduRestrictions)
            : name(std::move(name)), wage(wage), lowestLevel(lowestLevel), eduRestrictions(std::move(eduRestrictions))
        {
        }

        QString name;
        int wage;
        int lowestLevel;
        std::vector<QString> eduRestrictions;
    };

signals:
    // 当开始工作成功时发射该信号
    void WorkBeginSucceeded();
    // 当开始工作失败时发射该信号
    void WorkBeginFailed();
    // 当结束工作成功时发射该信号
    void WorkEndSucceeded();
    // 当结束工作成功时发射该信号
    void WorkEndFailed();
    void WorkNotBegins();

    // 当获取工作状态失败时发射该信号
    // 参数：str -> 错误信息
    void GetWorkingStatusFailed(const QString &str);

private:
    std::vector<JobInformation> jobs;       // 存放所有的工作信息，一开始先读取缓存中的，然后和服务器同步校正

    QString selectedJob;                    // 目前选择了哪个工作

    QWidget *lowerBarWidget;                // 打工页面底部的小容器，目前有“开始”和“结束”打工的按钮
    QHBoxLayout *lowerBarWidgetLayout;      // 打工页面底部的小容器的布局
    QPushButton *beginButton, *endButton;   // 打工页面底部的小容器中的开始打工和结束打工按钮
};

#endif // WORKINGWINDOW_H
