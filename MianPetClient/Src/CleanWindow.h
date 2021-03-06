﻿#ifndef CLEANWINDOW_H
#define CLEANWINDOW_H

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
#include <QString>

#include "SmallItemsContainerWindow.h"
#include "PetStatusData.h"
#include "ItemInformation.h"

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/CleanShopRequestData.h"
#include "Tools/Network/DataPackages/CleanValueRequeastData.h"
#include "Tools/Network/DataPackages/BuyRequestData.h"
#include "Tools/Network/DataPackages/UseRequestData.h"
#include "Tools/Detail/Calculation.h"

// 清洁品的商店
class CleanWindow final : public SmallItemsContainerWindow
{
    Q_OBJECT
public:
    explicit CleanWindow(QWidget *parent = nullptr);
    ~CleanWindow();

private:
    void InitializeUi();
    void InitializeConnect();

    void DataPrepare();

    void ViewPreviousPage();    // 展示清洁品列表的上一页
    void ViewNextPage();        // 展示清洁品列表的下一页

    void OnMouseHoversMoveItem(QObject *obj);
    void OnMouseHoversLeaveItem(QObject *obj);

    void OnBuyButtonClicked();
    void OnUseButtonClicked();

    void RequestDataInRange(int rangeBegin, int rangeEnd);

    void UpdateCleanValue();

signals:
    // 购买count个item成功时发出此信号
    void BuySucceeded(const QString &item, int count);
    // 购买count个item失败时发出此信号
    void BuyFailed(const QString &item, int count);

    // 使用count个item成功时发出此信号
    void UseSucceeded(const QString &item, int count);
    // 使用count个item失败时发出此信号
    void UseFailed(const QString &item, int count);

private:
    std::vector<ItemInformation> items{};   // 存放所有清洁品信息的容器

    QWidget *lowerBarWidget;                // 底部小容器，展示清洁值、文字、购买按钮和使用按钮的地方
    QHBoxLayout *lowerBarWidgetLayout;      // 底部小容器的布局
    QLabel *lowerBarTextLabel;              // 底部小容器中显示“清洁值”文字的地方
    QProgressBar *cleanBar;                 // 底部小容器中显示清洁值数值的地方
    QPushButton *buyButton, *useButton;     // 底部小容器中的“购买”和“使用”按钮

    QString selectedClean;                  //  选中的日用品，准备购买/使用（为了保持一致语法错误就先不管了）
};

#endif // CLEANWINDOW_H
