#ifndef TRANSPARENTITEMLABEL_H
#define TRANSPARENTITEMLABEL_H

#include <QLabel>

#ifdef Q_OS_WIN
    #pragma execution_character_set("utf-8")
#endif // Q_OS_WIN

class TransparentItemLabel : public QLabel
{
    Q_OBJECT
public:
    explicit TransparentItemLabel(QWidget *parent = nullptr);

signals:

};

#endif // TRANSPARENTITEMLABEL_H
