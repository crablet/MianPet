#ifndef PETPROFILEWINDOW_H
#define PETPROFILEWINDOW_H

#include <QWidget>

#include "FramelessWindow.h"

class PetProfileWindow final : public FramelessWindow
{
    Q_OBJECT
public:
    explicit PetProfileWindow(QWidget *parent = nullptr);

};

#endif // PETPROFILEWINDOW_H
