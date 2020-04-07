﻿#ifndef UICONSTANTS_H
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

#endif // UICONSTANTS_H
