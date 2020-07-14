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

#include "SmallItemsContainerWindow.h"
#include "PetStatusData.h"

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/CleanShopRequestData.h"

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

#endif // CLEANWINDOW_H
