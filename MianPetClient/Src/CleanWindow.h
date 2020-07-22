#ifndef CLEANWINDOW_H
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

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/CleanShopRequestData.h"
#include "Tools/Network/DataPackages/BuyRequestData.h"

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
    void BuySucceeded(const QString &item, int count);
    void BuyFailed(const QString &item, int count);

private:
    std::vector<ItemInformation> items{};

    QWidget *lowerBarWidget;    // 展示饥饿值、文字、购买按钮和使用按钮的地方
    QHBoxLayout *lowerBarWidgetLayout;
    QLabel *lowerBarTextLabel;
    QProgressBar *valueBar;
    QPushButton *buyButton, *useButton;

    QString selectedClean;  //  选中的日用品，准备购买/使用（为了保持一致语法错误就先不管了）
};

#endif // CLEANWINDOW_H
