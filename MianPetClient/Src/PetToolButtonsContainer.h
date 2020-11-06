#ifndef PETTOOLBUTTONSCONTAINER_H
#define PETTOOLBUTTONSCONTAINER_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>

#include "Src/Config/UiConstants.h"

#include "FoodWindow.h"
#include "CleanWindow.h"
#include "WorkingWindow.h"
#include "PetProfileWindow.h"

class PetToolButtonsContainer : public QWidget
{
    Q_OBJECT
public:
    explicit PetToolButtonsContainer(QWidget *parent = nullptr);

private:
    void InitializeUi();
    void InitializeConnect();

private slots:
    void OnFoodButtonClicked();     // 当点击喂食按钮时会发出此信号
    void OnCleanButtonClicked();    // 当点击清洁按钮时会发出此信号
    void OnWorkingButtonClicked();  // 当点击打工按钮时会发出此信号
    void OnPetProfileButtonClicked();   // 当点击面宠属性菜单时会发出此信号

private:
    QHBoxLayout *layout;    // 主体布局，呈一字型排开
    QPushButton *foodButton, *cleanButton, *petProfileButton, *workingButton;
    FoodWindow *foodWindow;         // 食物窗口，为人为保证的单例
    CleanWindow *cleanWindow;       // 洗澡窗口，为人为保证的单例
    WorkingWindow *workingWindow;   // 打工窗口，为人为保证的单例
    PetProfileWindow *petProfileWindow;
};

#endif // PETTOOLBUTTONSCONTAINER_H
