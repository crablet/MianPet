#ifndef PETTOOLBUTTONSCONTAINER_H
#define PETTOOLBUTTONSCONTAINER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

#include "SmallItemsContainerWindow.h"

class PetToolButtonsContainer : public QWidget
{
    Q_OBJECT
public:
    explicit PetToolButtonsContainer(QWidget *parent = nullptr);

private:
    QHBoxLayout *layout;
    QPushButton *button1, *button2, *button3, *button4;
    SmallItemsContainerWindow *smallWindow;
};

#endif // PETTOOLBUTTONSCONTAINER_H
