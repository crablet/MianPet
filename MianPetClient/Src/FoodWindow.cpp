﻿#include "FoodWindow.h"

FoodWindow::FoodWindow(QWidget *parent) : SmallItemsContainerWindow(parent)
{
    InitializeUi();
}

void FoodWindow::InitializeUi()
{
    SetIcon(FoodWindowIcon);
    SetWindowTitle("喂食");
}