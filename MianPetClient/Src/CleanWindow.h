#ifndef CLEANWINDOW_H
#define CLEANWINDOW_H

#include <thread>
#include <memory>
#include <atomic>
#include <algorithm>

#include <QWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QTcpSocket>

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

private:
    inline static QJsonArray itemNames{};
};

#endif // CLEANWINDOW_H
