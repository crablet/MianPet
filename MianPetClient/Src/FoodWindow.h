﻿#ifndef FOODWINDOW_H
#define FOODWINDOW_H

#include <thread>
#include <memory>
#include <atomic>
#include <algorithm>
#include <vector>

#include <QWidget>
#include <QHBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QTcpSocket>
#include <QIcon>
#include <QMouseEvent>
#include <QProgressBar>

#include "SmallItemsContainerWindow.h"

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/FoodShopRequestData.h"

class FoodWindow final : public SmallItemsContainerWindow
{
    Q_OBJECT
public:
    explicit FoodWindow(QWidget *parent = nullptr);
    ~FoodWindow();

private:
    void InitializeUi();
    void InitializeConnect();

    void DataPrepare();

    void ViewPreviousPage();
    void ViewNextPage();

    void OnMouseHoversMoveItem(QObject *obj);
    void OnMouseHoversLeaveItem(QObject *obj);

    void RequestDataInRange(int rangeBegin, int rangeEnd);

    struct ItemInformation
    {
        ItemInformation(const QString &name, int price, int amount)
            : name(name), price(price), amount(amount)
        {
        }

        QString name;
        int price;
        int amount;
    };

private:
    std::vector<ItemInformation> items{};

    QWidget *lowerBarWidget;    // 展示饥饿值、文字、购买按钮和使用按钮的地方
    QHBoxLayout *lowerBarWidgetLayout;
    QLabel *lowerBarTextLabel;
    QProgressBar *valueBar;
    QPushButton *buyButton, *useButton;
};

#endif // FOODWINDOW_H
