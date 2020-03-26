#ifndef PETCLIENT_H
#define PETCLIENT_H

#include <QWidget>
#include <QMouseEvent>
#include <QMovie>
#include <QLabel>
#include <QTimer>

#include "PetToolButtonsContainer.h"
#include "Src/Config/UiConstants.h"

class PetClient : public QWidget
{
    Q_OBJECT

public:
    PetClient(QWidget *parent = nullptr);

private:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    QMovie *petGif;
    QLabel *petGifLabel;
    QPoint previousMousePos, previousUiPos;

    PetToolButtonsContainer *petToolButtonsContainer;

    bool isUiBeingDragging;
};
#endif // PETCLIENT_H
