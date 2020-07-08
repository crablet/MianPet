#include "SmallItemsContainerWindow.h"

SmallItemsContainerWindow::SmallItemsContainerWindow(QWidget *parent) 
    : FramelessWindow(parent), currentPage(0)
{
    InitializeUi();
    InitializeConnect();
}

void SmallItemsContainerWindow::InitializeUi()
{
    setFixedSize(SmallItemsContainerWindowWidth, SmallItemsContainerWindowHeight);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() | Qt::Tool);   // Qt::Tool属性使得其任务栏图标被隐藏
    setMouseTracking(true); // 使得鼠标悬浮时能够及时更新状态

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
    item0->setFlat(true);
    item0->setStyleSheet("QPushButton { border: none }");
    item0->setIconSize(QSize(SmallItemsContainerWindowItem0IconWidth, SmallItemsContainerWindowItem0IconHeight));
    item0->installEventFilter(this);

    item1 = new QPushButton(this);
    item1->move(SmallItemsContainerWindowItem1X, SmallItemsContainerWindowItem1Y);
    item1->setFixedSize(SmallItemsContainerWindowItem1Width, SmallItemsContainerWindowItem1Height);
    item1->setFlat(true);
    item1->setStyleSheet("QPushButton { border: none }");
    item1->setIconSize(QSize(SmallItemsContainerWindowItem1IconWidth, SmallItemsContainerWindowItem1IconHeight));
    item1->installEventFilter(this);

    item2 = new QPushButton(this);
    item2->move(SmallItemsContainerWindowItem2X, SmallItemsContainerWindowItem2Y);
    item2->setFixedSize(SmallItemsContainerWindowItem2Width, SmallItemsContainerWindowItem2Height);
    item2->setFlat(true);
    item2->setStyleSheet("QPushButton { border: none }");
    item2->setIconSize(QSize(SmallItemsContainerWindowItem2IconWidth, SmallItemsContainerWindowItem2IconHeight));
    item2->installEventFilter(this);

    item3 = new QPushButton(this);
    item3->move(SmallItemsContainerWindowItem3X, SmallItemsContainerWindowItem3Y);
    item3->setFixedSize(SmallItemsContainerWindowItem3Width, SmallItemsContainerWindowItem3Height);
    item3->setFlat(true);
    item3->setStyleSheet("QPushButton { border: none }");
    item3->setIconSize(QSize(SmallItemsContainerWindowItem3IconWidth, SmallItemsContainerWindowItem3IconHeight));
    item3->installEventFilter(this);

    itemLabel = new TransparentItemLabel;
}

void SmallItemsContainerWindow::InitializeConnect()
{
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);  // hide or close or quit?
}

bool SmallItemsContainerWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == static_cast<QObject*>(item0)
     || object == static_cast<QObject*>(item1)
     || object == static_cast<QObject*>(item2)
     || object == static_cast<QObject*>(item3))
    {
        if (event->type() == QEvent::HoverEnter)
        {
            itemLabel->show();
            itemLabel->move(QCursor::pos() + QPoint(2, 2)); 
            // 得预留一些位置，不然会持续触发Enter和Leave事件，从而出现闪烁的情况
        }
        else if (event->type() == QEvent::HoverLeave)
        {
            itemLabel->hide();
        }
    }

    return QWidget::eventFilter(object, event);
}

// size默认是QSize(0, 0)，即如果不填第二个字段则默认使用iconLabel大小强行调整
// 如果指明了size则使用此大小
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
