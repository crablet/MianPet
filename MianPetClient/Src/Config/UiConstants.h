#ifndef UICONSTANTS_H
#define UICONSTANTS_H

#include <QSize>
#include <QIcon>
#include <QPixmap>

#define PetToolButtonsContainerWidth (this->width())
inline constexpr int PetToolButtonsContainerHeight = 66;
inline constexpr int PetToolButtonsContainerUiDelta = 16;
inline constexpr int PetToolButtonsContainerStayingTime = 520;
inline const QSize PetToolButtonsContainerButtonSize = QSize(43, 66);
#define PetToolButtonsContainerFoodButtonIcon (QIcon(":/Pic/bow-mix.png"))
#define PetToolButtonsContainerWaterButtonIcon (QIcon(":/Pic/clean-mix.png"))

inline constexpr int SmallItemsContainerWindowWidth = 440;
inline constexpr int SmallItemsContainerWindowHeight = 196;

#define SmallItemsContainerWindowBackgroundPixmap (QPixmap(":/Pic/container-background.png"))

#define SmallItemsContainerWindowCloseButtonIcon (QIcon(":/Pic/close.png"))
inline constexpr int SmallItemsContainerWindowCloseButtonWidth = 25;
inline constexpr int SmallItemsContainerWindowCloseButtonHeight = 25;
inline constexpr int SmallItemsContainerWindowCloseButtonX = 412;
inline constexpr int SmallItemsContainerWindowCloseButtonY = 2;

inline constexpr int SmallItemsContainerWindowIconLabelWidth = 35;
inline constexpr int SmallItemsContainerWindowIconLabelHeight = 35;
inline constexpr int SmallItemsContainerWindowIconLabelX = 0;
inline constexpr int SmallItemsContainerWindowIconLabelY = 0;

inline constexpr int SmallItemsContainerWindowWindowTitleLabelX 
    = SmallItemsContainerWindowIconLabelX + SmallItemsContainerWindowIconLabelWidth + 5;
inline constexpr int SmallItemsContainerWindowWindowTitleLabelY
    = SmallItemsContainerWindowIconLabelY;
inline constexpr int SmallItemsContainerWindowWindowTitleLabelWidth = 199;
inline constexpr int SmallItemsContainerWindowWindowTitleLabelHeight = SmallItemsContainerWindowIconLabelHeight;

inline constexpr int SmallItemsContainerWindowPreviousPageButtonWidth = 30;
inline constexpr int SmallItemsContainerWindowPreviousPageButtonHeight = 40;
inline constexpr int SmallItemsContainerWindowPreviousPageButtonX = 6;
inline constexpr int SmallItemsContainerWindowPreviousPageButtonY = SmallItemsContainerWindowHeight / 2;
#define SmallItemsContainerWindowPreviousPageButtonIcon (QIcon(":/Pic/left-arrow.png"))

inline constexpr int SmallItemsContainerWindowNextPageButtonWidth = SmallItemsContainerWindowPreviousPageButtonWidth;
inline constexpr int SmallItemsContainerWindowNextPageButtonHeight = SmallItemsContainerWindowPreviousPageButtonHeight;
inline constexpr int SmallItemsContainerWindowNextPageButtonX 
    = SmallItemsContainerWindowWidth - SmallItemsContainerWindowPreviousPageButtonX - SmallItemsContainerWindowPreviousPageButtonWidth;
inline constexpr int SmallItemsContainerWindowNextPageButtonY = SmallItemsContainerWindowPreviousPageButtonY;
#define SmallItemsContainerWindowNextPageButtonIcon (QIcon(":/Pic/right-arrow.png"))

#define FoodWindowIcon (QPixmap(":/Pic/fork-and-knife2.png"))

inline constexpr int SmallItemsContainerWindowItem0X = 45;
inline constexpr int SmallItemsContainerWindowItem0Y = 42;
inline constexpr int SmallItemsContainerWindowItem0Width = 80;
inline constexpr int SmallItemsContainerWindowItem0Height = 105;
inline constexpr int SmallItemsContainerWindowItem0IconWidth = 100;
inline constexpr int SmallItemsContainerWindowItem0IconHeight = 100;

inline constexpr int SmallItemsContainerWindowItem1X 
    = SmallItemsContainerWindowItem0X + SmallItemsContainerWindowItem0Width + 11;
inline constexpr int SmallItemsContainerWindowItem1Y = SmallItemsContainerWindowItem0Y;
inline constexpr int SmallItemsContainerWindowItem1Width = SmallItemsContainerWindowItem0Width;
inline constexpr int SmallItemsContainerWindowItem1Height = SmallItemsContainerWindowItem0Height;
inline constexpr int SmallItemsContainerWindowItem1IconWidth = SmallItemsContainerWindowItem0IconWidth;
inline constexpr int SmallItemsContainerWindowItem1IconHeight = SmallItemsContainerWindowItem0IconHeight;

inline constexpr int SmallItemsContainerWindowItem2X 
    = SmallItemsContainerWindowItem1X + SmallItemsContainerWindowItem1Width + 11;
inline constexpr int SmallItemsContainerWindowItem2Y = SmallItemsContainerWindowItem0Y;
inline constexpr int SmallItemsContainerWindowItem2Width = SmallItemsContainerWindowItem0Width;
inline constexpr int SmallItemsContainerWindowItem2Height = SmallItemsContainerWindowItem0Height;
inline constexpr int SmallItemsContainerWindowItem2IconWidth = SmallItemsContainerWindowItem0IconWidth;
inline constexpr int SmallItemsContainerWindowItem2IconHeight = SmallItemsContainerWindowItem0IconHeight;

inline constexpr int SmallItemsContainerWindowItem3X
    = SmallItemsContainerWindowItem2X + SmallItemsContainerWindowItem2Width + 11;
inline constexpr int SmallItemsContainerWindowItem3Y = SmallItemsContainerWindowItem0Y;
inline constexpr int SmallItemsContainerWindowItem3Width = SmallItemsContainerWindowItem0Width;
inline constexpr int SmallItemsContainerWindowItem3Height = SmallItemsContainerWindowItem0Height;
inline constexpr int SmallItemsContainerWindowItem3IconWidth = SmallItemsContainerWindowItem0IconWidth;
inline constexpr int SmallItemsContainerWindowItem3IconHeight = SmallItemsContainerWindowItem0IconHeight;

inline constexpr int LoginWindowWidth = 537;
inline constexpr int LoginWindowHeight = 412;

inline constexpr int LoginWindowAccountLineEditWidth = LoginWindowWidth - 121 * 2;
inline constexpr int LoginWindowAccountLineEditHeight = 44;
inline constexpr int LoginWindowAccountLineEditX = 121;
inline constexpr int LoginWindowAccountLineEditY = 199;

inline constexpr int LoginWindowPasswordLineEditWidth = LoginWindowAccountLineEditWidth;
inline constexpr int LoginWindowPasswordLineEditHeight = LoginWindowAccountLineEditHeight;
inline constexpr int LoginWindowPasswordLineEditX = LoginWindowAccountLineEditX;
inline constexpr int LoginWindowPasswordLineEditY = LoginWindowAccountLineEditY + LoginWindowAccountLineEditHeight + 6;

inline constexpr int LoginWindowLoginButtonWidth = LoginWindowPasswordLineEditWidth;
inline constexpr int LoginWindowLoginButtonHeight = LoginWindowPasswordLineEditHeight;
inline constexpr int LoginWindowLoginButtonX = LoginWindowPasswordLineEditX;
inline constexpr int LoginWindowLoginButtonY = LoginWindowPasswordLineEditY + LoginWindowPasswordLineEditHeight + 50;

inline constexpr int LoginWindowCloseButtonWidth = 40;
inline constexpr int LoginWindowCloseButtonHeight = 40;
inline constexpr int LoginWindowCloseButtonX = LoginWindowWidth - LoginWindowCloseButtonWidth;
inline constexpr int LoginWindowCloseButtonY = 0;

inline constexpr int LoginWindowRegisterButtonWidth = 77;
inline constexpr int LoginWindowRegisterButtonHeight = 32;
inline constexpr int LoginWindowRegisterButtonX = 4;
inline constexpr int LoginWindowRegisterButtonY 
    = LoginWindowHeight - LoginWindowRegisterButtonHeight - LoginWindowRegisterButtonX;

inline constexpr int PetProfileWindowWidth = 204;
inline constexpr int PetProfileWindowHeight = 327;

inline constexpr int TransparentItemLabelWidth = 167;
inline constexpr int TransparentItemLabelHeight = 123;

inline constexpr int TransparentItemLabelUpperLabelWidth = 140;
inline constexpr int TransparentItemLabelUpperLabelHeight = 24;
inline constexpr int TransparentItemLabelUpperLabelX = 12;
inline constexpr int TransparentItemLabelUpperLabelY = 12;
inline const QString TransparentItemLabelUpperLabelStyle 
    = R"(QLabel { color: rgb(247, 247, 190); font-size: 22px })";

inline constexpr int TransparentItemLabelLowerLabelWidth = TransparentItemLabelUpperLabelWidth;
inline constexpr int TransparentItemLabelLowerLabelHeight = 56;
inline constexpr int TransparentItemLabelLowerLabelX = TransparentItemLabelUpperLabelX;
inline constexpr int TransparentItemLabelLowerLabelY = 64;
inline const QString TransparentItemLabelLowerLabelStyle = "";

inline constexpr int FoodWindowLowerBarWidgetWidth 
    = SmallItemsContainerWindowItem3X + SmallItemsContainerWindowItem3IconWidth - SmallItemsContainerWindowItem0X;
inline constexpr int FoodWindowLowerBarWidgetHeight = 53;
inline constexpr int FoodWindowLowerBarWidgetX = SmallItemsContainerWindowItem0X;
inline constexpr int FoodWindowLowerBarWidgetY 
    = SmallItemsContainerWindowItem0Y + SmallItemsContainerWindowItem0Height + 3;

inline constexpr int CleanWindowLowerBarWidgetWidth = FoodWindowLowerBarWidgetWidth;
inline constexpr int CleanWindowLowerBarWidgetHeight = FoodWindowLowerBarWidgetHeight;
inline constexpr int CleanWindowLowerBarWidgetX = FoodWindowLowerBarWidgetX;
inline constexpr int CleanWindowLowerBarWidgetY = FoodWindowLowerBarWidgetY;

inline constexpr int WorkingWindowLowerBarWidgetWidth = FoodWindowLowerBarWidgetWidth;
inline constexpr int WorkingWindowLowerBarWidgetHeight = FoodWindowLowerBarWidgetHeight;
inline constexpr int WorkingWindowLowerBarWidgetX = FoodWindowLowerBarWidgetX;
inline constexpr int WorkingWindowLowerBarWidgetY = FoodWindowLowerBarWidgetY;

inline constexpr int SettingsWindowCloseButtonWidth = 20;
inline constexpr int SettingsWindowCloseButtonHeight = SettingsWindowCloseButtonWidth;

#endif // UICONSTANTS_H
