#ifndef PETPROFILEWINDOW_H
#define PETPROFILEWINDOW_H

#include <QWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QSizePolicy>
#include <QTimer>
#include <QEvent>

#include "Src/Config/UiConstants.h"

#include "FramelessWindow.h"

class PetProfileWindow final : public FramelessWindow
{
    Q_OBJECT
public:
    explicit PetProfileWindow(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();

    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    QWidget *upperWidget, *lowerWidget;
    QFormLayout *upperLayout;
    QVBoxLayout *lowerLayout, *allLayout;
    QLabel *nicknameLabel, *idLabel, *levelLabel, *ageLabel, *grouthSpeedLabel, *statusLabel, *onlineTimeLabel;
    QProgressBar *growthBar, *foodBar, *cleanBar, *healthBar, *moodBar;
    QTimer *closeCountdownTimer;
};

#endif // PETPROFILEWINDOW_H
