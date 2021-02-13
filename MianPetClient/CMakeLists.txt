cmake_minimum_required(VERSION 3.1.0)

project(MianPetClient VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

if (UNIX)
    link_libraries(pthread)
ENDIF()

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(SOURCES
    Src/CleanWindow.cpp
    Src/FoodWindow.cpp
    Src/FramelessWindow.cpp
    Src/LoginWindow.cpp
    Src/PetProfileWindow.cpp
    Src/PetToolButtonsContainer.cpp
    Src/SettingsWindow.cpp
    Src/SmallItemsContainerWindow.cpp
    Src/Tools/Network/DataPackages/BuyRequestData.cpp
    Src/Tools/Network/DataPackages/CleanShopRequestData.cpp
    Src/Tools/Network/DataPackages/CorekeyRequestData.cpp
    Src/Tools/Network/DataPackages/DataPackage.cpp
    Src/Tools/Network/DataPackages/FoodShopRequestData.cpp
    Src/Tools/Network/DataPackages/HeartbeatData.cpp
    Src/Tools/Network/DataPackages/JobsInfoRequestData.cpp
    Src/Tools/Network/DataPackages/LoginRequestData.cpp
    Src/Tools/Network/DataPackages/LogoutData.cpp
    Src/Tools/Network/DataPackages/PetProfileRequestData.cpp
    Src/Tools/Network/DataPackages/RequestData.cpp
    Src/Tools/Network/DataPackages/UseRequestData.cpp
    Src/Tools/Network/DataPackages/WorkBeginRequest.cpp
    Src/Tools/Network/DataPackages/WorkEndRequest.cpp
    Src/Tools/Network/DataPackages/WorkStatusRequestData.cpp
    Src/TransparentItemLabel.cpp
    Src/WorkingWindow.cpp
    Src/main.cpp
    Src/PetClient.cpp
    Src/ItemInformation.cpp
    Src/Tools/Network/DataPackages/FoodValueRequestData.cpp
    Src/Tools/Network/DataPackages/CleanValueRequeastData.cpp)

SET(HEADER_DIRS
    Src
    Src/Config
    Src/Tools/Log/spdlog-1.8.2
    Src/Tools/Network
    Src/Tools/Network/DataPackages
    Src/Tools/Ui)
include_directories(${HEADER_DIRS})

add_executable(MianPetClient ${SOURCES} Res.qrc)
target_link_libraries(MianPetClient Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)

