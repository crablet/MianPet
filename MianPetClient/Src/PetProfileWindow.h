#ifndef PETPROFILEWINDOW_H
#define PETPROFILEWINDOW_H

#include <memory>
#include <thread>

#include <QWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QSizePolicy>
#include <QTimer>
#include <QEvent>
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QPaintEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QColor>

#include "Src/Config/UiConstants.h"
#include "Src/Config/NetworkConstants.h"
#include "Src/Tools/Network/DataPackages/PetProfileRequestData.h"

#include "PetStatusData.h"

#include "FramelessWindow.h"

class PetProfileWindow final : public FramelessWindow
{
    Q_OBJECT
public:
    explicit PetProfileWindow(QWidget *parent = nullptr);

signals:
    // 如果profile为空，则表示传输过程中出现了差错；如果不为空，则profile中为tcp解包后的内容
    // 可以改进的地方：profile为空表示出错可以细化成多个信号，只是为空不能表明出错的原因
    void CanUpdatePetProfile(const QByteArray &profile);

private:
    void InitializeUi();
    void InitializeConnect();

    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

    void UpdatePetProfile();
    void UpdatePetProfileHelper(const QByteArray &profile);

private:
    QWidget *upperWidget, *lowerWidget;
    QFormLayout *upperLayout;
    QVBoxLayout *lowerLayout, *allLayout;
    QLabel *usernameLabel, *idLabel, *levelLabel, *ageLabel, *grouthSpeedLabel, *statusLabel, *onlineTimeLabel;
    QProgressBar *growthBar, *foodBar, *cleanBar, *healthBar, *moodBar;
    QTimer *closeCountdownTimer;    // 用于实现鼠标移开则自动关闭窗口的定时器
};

#endif // PETPROFILEWINDOW_H
