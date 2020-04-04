#ifndef CLEANWINDOW_H
#define CLEANWINDOW_H

#include <QWidget>

#include "SmallItemsContainerWindow.h"

class CleanWindow final : public SmallItemsContainerWindow
{
    Q_OBJECT
public:
    explicit CleanWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
};

#endif // CLEANWINDOW_H
