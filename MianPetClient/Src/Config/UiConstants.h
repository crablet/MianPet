﻿#ifndef UICONSTANTS_H
#define UICONSTANTS_H

#include <QSize>

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

#endif // UICONSTANTS_H
