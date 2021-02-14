#include "TransparentItemLabel.h"

TransparentItemLabel::TransparentItemLabel(QWidget *parent) : QWidget(parent)
{
    InitializeUi();
    InitializeConnect();
}

void TransparentItemLabel::SetUpperLabelText(const QString &str)
{
    upperLabel->setText(str);
}

void TransparentItemLabel::SetLowerLabelText(const QString &str)
{
    lowerLabel->setText(str);
}

QLabel* TransparentItemLabel::GetUpperLabel() const
{
    return upperLabel;
}

QLabel* TransparentItemLabel::GetLowerLabel() const
{
    return lowerLabel;
}

void TransparentItemLabel::InitializeUi()
{
    setFixedSize(TransparentItemLabelWidth, TransparentItemLabelHeight);
#ifdef Q_OS_LINUX
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::Tool); // Qt::Tool属性使得其任务栏图标被隐藏
#else
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);  // Qt::Tool属性使得其任务栏图标被隐藏
#endif // OS_LINUX
    setWindowOpacity(0.8);  // FIXME: 这样写会让文字也变成透明的，需要只是窗口透明而控件不透明

    upperLabel = new QLabel(this);
    upperLabel->setFixedSize(TransparentItemLabelUpperLabelWidth, TransparentItemLabelUpperLabelHeight);
    upperLabel->move(TransparentItemLabelUpperLabelX, TransparentItemLabelUpperLabelY);
    upperLabel->setStyleSheet(TransparentItemLabelUpperLabelStyle);

    lowerLabel = new QLabel(this);
    lowerLabel->setFixedSize(TransparentItemLabelLowerLabelWidth, TransparentItemLabelLowerLabelHeight);
    lowerLabel->move(TransparentItemLabelLowerLabelX, TransparentItemLabelLowerLabelY);
    lowerLabel->setStyleSheet(TransparentItemLabelLowerLabelStyle);
}

void TransparentItemLabel::InitializeConnect()
{
}

void TransparentItemLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(123, 94, 66));
    painter.drawRect(this->rect());
    painter.setPen(QColor(255, 172, 0));
    painter.drawLine(TransparentItemLabelUpperLabelX, 49, TransparentItemLabelWidth - TransparentItemLabelUpperLabelX, 49);

    QWidget::paintEvent(event);
}
