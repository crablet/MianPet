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

#include "FramelessWindow.h"

class PetProfileWindow final : public FramelessWindow
{
    Q_OBJECT
public:
    explicit PetProfileWindow(QWidget *parent = nullptr);

signals:
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
    QLabel *nicknameLabel, *idLabel, *levelLabel, *ageLabel, *grouthSpeedLabel, *statusLabel, *onlineTimeLabel;
    QProgressBar *growthBar, *foodBar, *cleanBar, *healthBar, *moodBar;
    QTimer *closeCountdownTimer;
};

#endif // PETPROFILEWINDOW_H
