#ifndef TRANSPARENTITEMLABEL_H
#define TRANSPARENTITEMLABEL_H

#include <QWidget>
#include <QPalette>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QString>

#include "Src/Config/UiConstants.h"

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

// 一种透明背景的控件，主要用于展示各食品、清洁和工种等项目
class TransparentItemLabel : public QWidget
{
    Q_OBJECT
public:
    explicit TransparentItemLabel(QWidget *parent = nullptr);

public:
    void SetUpperLabelText(const QString &str);
    void SetLowerLabelText(const QString &str);

    QLabel* GetUpperLabel() const;
    QLabel* GetLowerLabel() const;

private:
    void InitializeUi();
    void InitializeConnect();

    void paintEvent(QPaintEvent *event) override;

protected:
    QLabel *upperLabel, *lowerLabel;
};

#endif // TRANSPARENTITEMLABEL_H
