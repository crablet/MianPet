#ifndef WORKINGWINDOW_H
#define WORKINGWINDOW_H

#include "SmallItemsContainerWindow.h"

class WorkingWindow final : public SmallItemsContainerWindow
{
public:
    WorkingWindow();

private:
    void InitializeUi();
    void InitializeConnect();
};

#endif // WORKINGWINDOW_H
