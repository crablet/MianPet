#ifndef CLEANWINDOW_H
#define CLEANWINDOW_H

#include <thread>
#include <memory>
#include <atomic>
#include <algorithm>
#include <vector>

#include <QWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QTcpSocket>
#include <QIcon>

#include "SmallItemsContainerWindow.h"

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/CleanShopRequestData.h"

class CleanWindow final : public SmallItemsContainerWindow
{
    Q_OBJECT
public:
    explicit CleanWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();

    void DataPrepare();

    void ViewPreviousPage();
    void ViewNextPage();

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
    inline static std::vector<ItemInformation> items{}; // first: name, second: price
};

#endif // CLEANWINDOW_H
