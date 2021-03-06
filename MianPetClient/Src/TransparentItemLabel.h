﻿#ifndef TRANSPARENTITEMLABEL_H
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
    void SetUpperLabelText(const QString &str); // 设置上半部分的QLabel的内容
    void SetLowerLabelText(const QString &str); // 设置下半部分的QLabel的内容

    QLabel* GetUpperLabel() const;  // 返回上半部分的QLabel
    QLabel* GetLowerLabel() const;  // 返回下半部分的QLabel

private:
    void InitializeUi();
    void InitializeConnect();

    void paintEvent(QPaintEvent *event) override;

protected:
    QLabel *upperLabel; // 上半部分的QLabel
    QLabel *lowerLabel; // 下半部分的QLabel
};

#endif // TRANSPARENTITEMLABEL_H
