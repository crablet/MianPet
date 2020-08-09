#ifndef WORKINGWINDOW_H
#define WORKINGWINDOW_H

#include <vector>
#include <thread>
#include <algorithm>

#include <QString>
#include <QJsonArray>
#include <QJsonObject>

#include "SmallItemsContainerWindow.h"

#include "Tools/Network/DataPackages/WorkBeginRequest.h"
#include "Tools/Network/DataPackages/WorkEndRequest.h"

class WorkingWindow final : public SmallItemsContainerWindow
{
public:
    WorkingWindow();
    ~WorkingWindow();

private:
    void InitializeUi();
    void InitializeConnect();

    void ViewPreviousPage();
    void ViewNextPage();

    void OnMouseHoversMoveItem(QObject *obj);
    void OnMouseHoversLeaveItem(QObject *obj);

    void RequestDataInRange(int rangeBegin, int rangeEnd);
    void SubmitWorkBeginRequest(const std::string &jobName);
    void SubmitWorkEndRequest(const std::string &jobName);

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

private:
    std::vector<JobInformation> jobs;

    QString selectedJob;
};

#endif // WORKINGWINDOW_H
