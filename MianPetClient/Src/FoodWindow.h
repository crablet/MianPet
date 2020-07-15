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

private:
    std::vector<ItemInformation> items{};

    QWidget *lowerBarWidget;    // 展示饥饿值、文字、购买按钮和使用按钮的地方
    QHBoxLayout *lowerBarWidgetLayout;
    QLabel *lowerBarTextLabel;
    QProgressBar *valueBar;
    QPushButton *buyButton, *useButton;

    QString selectedFood;   // 选中的食品，准备购买/使用
};

#endif // FOODWINDOW_H
