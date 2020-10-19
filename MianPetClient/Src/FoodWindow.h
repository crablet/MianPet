#ifndef FOODWINDOW_H
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
#include <QString>

#include "SmallItemsContainerWindow.h"
#include "PetStatusData.h"

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/FoodShopRequestData.h"
#include "Tools/Network/DataPackages/BuyRequestData.h"
#include "Tools/Network/DataPackages/UseRequestData.h"

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

    void OnBuyButtonClicked();
    void OnUseButtonClicked();

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

signals:
    // 购买count个item成功
    void BuySucceeded(const QString &item, int count);
    // 购买count个item失败
    void BuyFailed(const QString &item, int count);

    // 使用count个item成功
    void UseSucceeded(const QString &item, int count);
    // 使用count个item失败
    void UseFailed(const QString &item, int count);

private:
    std::vector<ItemInformation> items{};   // 存放所有食品的容器

    QWidget *lowerBarWidget;    // 展示饥饿值、文字、购买按钮和使用按钮的地方
    QHBoxLayout *lowerBarWidgetLayout;  // 底部小容器的布局
    QLabel *lowerBarTextLabel;          // 底部小容器中显示“饥饿值”文字的地方
    QProgressBar *valueBar;             // 底部小容器中显示饥饿值数值的地方
    QPushButton *buyButton, *useButton; // 底部小容器中的“购买”和“使用”按钮

    QString selectedFood;   // 选中的食品，准备购买/使用
};

#endif // FOODWINDOW_H
