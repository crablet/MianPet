#ifndef FOODWINDOW_H
#define FOODWINDOW_H

#include <thread>
#include <memory>
#include <atomic>

#include <QWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QTcpSocket>

#include "SmallItemsContainerWindow.h"

#include "Config/NetworkConstants.h"
#include "Tools/Network/DataPackages/FoodShopRequestData.h"

class FoodWindow final : public SmallItemsContainerWindow
{
    Q_OBJECT
public:
    explicit FoodWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();

    void DataPrepare();

    void ViewPreviousPage();
    void ViewNextPage();

private:
    inline static QJsonArray itemNames{};
};

#endif // FOODWINDOW_H
