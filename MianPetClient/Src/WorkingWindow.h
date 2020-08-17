#ifndef WORKINGWINDOW_H
#define WORKINGWINDOW_H

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

    void RequestJobsInfoInRange(int rangeBegin, int rangeEnd);
    void SubmitWorkBeginRequest(const QString &jobName);
    void SubmitWorkEndRequest(const QString &jobName);

    void GetWorkingStatus();

    struct JobInformation
    {
        JobInformation(const QString &name, int wage, int lowestLevel, const std::vector<QString> &eduRestrictions)
            : name(name), wage(wage), lowestLevel(lowestLevel), eduRestrictions(eduRestrictions)
        {
        }

        QString name;
        int wage;
        int lowestLevel;
        std::vector<QString> eduRestrictions;
    };

signals:
    void WorkBeginSucceeded();
    void WorkBeginFailed();
    void WorkEndSucceeded();
    void WorkEndFailed();

private:
    std::vector<JobInformation> jobs;

    QString selectedJob;    // 目前选择了哪个工作

    QWidget *lowerBarWidget;    // 打工页面底部的小容器，目前有“开始”和“结束”打工的按钮
    QHBoxLayout *lowerBarWidgetLayout;
    QPushButton *beginButton, *endButton;
};

#endif // WORKINGWINDOW_H
