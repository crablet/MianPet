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

#include "SmallItemsContainerWindow.h"

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

    QString selectedJob;
};

#endif // WORKINGWINDOW_H
