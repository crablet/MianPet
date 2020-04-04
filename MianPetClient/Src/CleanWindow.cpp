#include "CleanWindow.h"

CleanWindow::CleanWindow(QWidget *parent) : SmallItemsContainerWindow(parent)
{
    InitializeUi();
}

void CleanWindow::InitializeUi()
{
    SetWindowTitle("清洁");
}
