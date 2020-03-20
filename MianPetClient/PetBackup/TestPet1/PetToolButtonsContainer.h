#ifndef PETTOOLBUTTONSCONTAINER_H
#define PETTOOLBUTTONSCONTAINER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class PetToolButtonsContainer : public QWidget
{
    Q_OBJECT
public:
    explicit PetToolButtonsContainer(QWidget *parent = nullptr);

private:
    QHBoxLayout *layout;
    QPushButton *button1, *button2;
};

#endif // PETTOOLBUTTONSCONTAINER_H
