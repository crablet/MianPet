#ifndef WORKINGWINDOW_H
#define WORKINGWINDOW_H

#include <QString>

#include "SmallItemsContainerWindow.h"

class WorkingWindow final : public SmallItemsContainerWindow
{
public:
    WorkingWindow();

private:
    void InitializeUi();
    void InitializeConnect();

    void ViewPreviousPage();
    void ViewNextPage();

    void OnMouseHoversMoveItem(QObject *obj);
    void OnMouseHoversLeaveItem(QObject *obj);

    struct JobInformation
    {
        JobInformation()
        {
        }
    };

private:
    QString selectedJob;
};

#endif // WORKINGWINDOW_H
