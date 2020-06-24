#ifndef FOODWINDOW_H
#define FOODWINDOW_H

#include <thread>
#include <memory>
#include <atomic>
#include <algorithm>
#include <vector>
#include <utility>

#include <QWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QTcpSocket>
#include <QIcon>

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
    inline static std::vector<std::pair<QString, int>> items{}; // first: name, second: price
};

#endif // FOODWINDOW_H
