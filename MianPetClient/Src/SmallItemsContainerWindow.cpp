#include "SmallItemsContainerWindow.h"

SmallItemsContainerWindow::SmallItemsContainerWindow(QWidget *parent) 
    : FramelessWindow(parent)
{
    InitializeUi();
    InitializeConnect();
}

void SmallItemsContainerWindow::InitializeUi()
{
    setFixedSize(SmallItemsContainerWindowWidth, SmallItemsContainerWindowHeight);
    setAttribute(Qt::WA_DeleteOnClose);

    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(SmallItemsContainerWindowBackgroundPixmap));
    setPalette(palette);

    closeButton = new QPushButton(this);
    closeButton->setIcon(SmallItemsContainerWindowCloseButtonIcon);
    closeButton->setIconSize(QSize(SmallItemsContainerWindowCloseButtonWidth, SmallItemsContainerWindowCloseButtonHeight));
    closeButton->move(SmallItemsContainerWindowCloseButtonX, SmallItemsContainerWindowCloseButtonY);
    closeButton->setFlat(true);
    closeButton->setStyleSheet("QPushButton{ border: none }");

    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(SmallItemsContainerWindowIconLabelWidth, SmallItemsContainerWindowIconLabelHeight);
    iconLabel->move(SmallItemsContainerWindowIconLabelX, SmallItemsContainerWindowIconLabelY);

    windowTitleLabel = new QLabel(this);
    windowTitleLabel->setFixedSize(SmallItemsContainerWindowWindowTitleLabelWidth, SmallItemsContainerWindowWindowTitleLabelHeight);
    windowTitleLabel->move(SmallItemsContainerWindowWindowTitleLabelX, SmallItemsContainerWindowWindowTitleLabelY);
    windowTitleLabel->setStyleSheet("QLabel { color: white }");

    previousPageButton = new QPushButton(this);
    previousPageButton->move(SmallItemsContainerWindowPreviousPageButtonX, SmallItemsContainerWindowPreviousPageButtonY);
    previousPageButton->setFlat(true);
    previousPageButton->setStyleSheet("QPushButton{ border: none }");
    previousPageButton->setIcon(SmallItemsContainerWindowPreviousPageButtonIcon);
    previousPageButton->setIconSize(QSize(SmallItemsContainerWindowPreviousPageButtonWidth, SmallItemsContainerWindowPreviousPageButtonHeight));

    nextPageButton = new QPushButton(this);
    nextPageButton->move(SmallItemsContainerWindowNextPageButtonX, SmallItemsContainerWindowNextPageButtonY);
    nextPageButton->setFlat(true);
    nextPageButton->setStyleSheet("QPushButton{ border: none }");
    nextPageButton->setIcon(SmallItemsContainerWindowNextPageButtonIcon);
    nextPageButton->setIconSize(QSize(SmallItemsContainerWindowNextPageButtonWidth, SmallItemsContainerWindowNextPageButtonHeight));

    item0 = new QPushButton(this);
    item0->move(SmallItemsContainerWindowItem0X, SmallItemsContainerWindowItem0Y);
    item0->setFixedSize(SmallItemsContainerWindowItem0Width, SmallItemsContainerWindowItem0Height);

    item1 = new QPushButton(this);
    item1->move(SmallItemsContainerWindowItem1X, SmallItemsContainerWindowItem1Y);
    item1->setFixedSize(SmallItemsContainerWindowItem1Width, SmallItemsContainerWindowItem1Height);

    item2 = new QPushButton(this);
    item2->move(SmallItemsContainerWindowItem2X, SmallItemsContainerWindowItem2Y);
    item2->setFixedSize(SmallItemsContainerWindowItem2Width, SmallItemsContainerWindowItem2Height);

    item3 = new QPushButton(this);
    item3->move(SmallItemsContainerWindowItem3X, SmallItemsContainerWindowItem3Y);
    item3->setFixedSize(SmallItemsContainerWindowItem3Width, SmallItemsContainerWindowItem3Height);
}

void SmallItemsContainerWindow::InitializeConnect()
{
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);  // hide or close or quit?
}

void SmallItemsContainerWindow::SetIcon(const QPixmap &icon, const QSize &size)
{
    if (size == QSize(0, 0))
    {
        iconLabel->setPixmap(icon.scaled(iconLabel->size()));
    }
    else
    {
        iconLabel->setPixmap(icon.scaled(size));
    }
}

void SmallItemsContainerWindow::SetWindowTitle(const QString &text)
{
    windowTitleLabel->setText(text);
}
