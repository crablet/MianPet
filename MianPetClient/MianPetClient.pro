QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += Src/Tools/Log/spdlog-1.7.0

SOURCES += \
    Src/CleanWindow.cpp \
    Src/FoodWindow.cpp \
    Src/FramelessWindow.cpp \
    Src/LoginWindow.cpp \
    Src/PetProfileWindow.cpp \
    Src/PetToolButtonsContainer.cpp \
    Src/SmallItemsContainerWindow.cpp \
    Src/Tools/Network/DataPackages/BuyRequestData.cpp \
    Src/Tools/Network/DataPackages/CleanShopRequestData.cpp \
    Src/Tools/Network/DataPackages/CorekeyRequestData.cpp \
    Src/Tools/Network/DataPackages/DataPackage.cpp \
    Src/Tools/Network/DataPackages/FoodShopRequestData.cpp \
    Src/Tools/Network/DataPackages/HeartbeatData.cpp \
    Src/Tools/Network/DataPackages/LoginRequestData.cpp \
    Src/Tools/Network/DataPackages/LogoutData.cpp \
    Src/Tools/Network/DataPackages/PetProfileRequestData.cpp \
    Src/Tools/Network/DataPackages/RequestData.cpp \
    Src/Tools/Network/DataPackages/UseRequestData.cpp \
    Src/TransparentItemLabel.cpp \
    Src/main.cpp \
    Src/PetClient.cpp

HEADERS += \
    Src/CleanWindow.h \
    Src/Config/NetworkConstants.h \
    Src/Config/UiConstants.h \
    Src/FoodWindow.h \
    Src/FramelessWindow.h \
    Src/LoginWindow.h \
    Src/PetClient.h \
    Src/PetProfileWindow.h \
    Src/PetStatusData.h \
    Src/PetToolButtonsContainer.h \
    Src/SmallItemsContainerWindow.h \
    Src/Tools/Network/DataPackages/BuyRequestData.h \
    Src/Tools/Network/DataPackages/CleanShopRequestData.h \
    Src/Tools/Network/DataPackages/CorekeyRequestData.h \
    Src/Tools/Network/DataPackages/DataPackage.h \
    Src/Tools/Network/DataPackages/FoodShopRequestData.h \
    Src/Tools/Network/DataPackages/HeartbeatData.h \
    Src/Tools/Network/DataPackages/LoginRequestData.h \
    Src/Tools/Network/DataPackages/LogoutData.h \
    Src/Tools/Network/DataPackages/PetProfileRequestData.h \
    Src/Tools/Network/DataPackages/RequestData.h \
    Src/Tools/Network/DataPackages/UseRequestData.h \
    Src/Tools/Network/NetworkTools.h \
    Src/Tools/Ui/UiTools.h \
    Src/TransparentItemLabel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Res.qrc
