cmake_minimum_required(VERSION 3.14)

project(G14ControlPP LANGUAGES CXX)
set(CMAKE_PREFIX_PATH d:\\Cpp_Dev\\Qt\\5.15.2\\mingw81_64\\)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(SYSTEM ./src ./src/atkacpi)

set(SOURCES
        src/main.cpp
        src/ui/MainWindow.cpp
        src/ui/MainWindow.h
        src/ui/mainTab/MainTab.cpp
        src/ui/mainTab/MainTab.h
        src/ryzenadj/RyzenAdjSingleton.cpp
        src/ryzenadj/RyzenAdjSingleton.h
        src/ryzenadj/RyzenAdjTypes.h
        src/ui/fansTab/FansTab.cpp
        src/ui/fansTab/FansTab.h
        src/atkacpi/AcpiControlSingleton.cpp
        src/atkacpi/AcpiControlSingleton.h src/atkacpi/AcpiListenerThread.cpp src/atkacpi/AcpiListenerThread.h src/events/GlobalEventDispatcher.cpp src/events/GlobalEventDispatcher.h src/kbd/KbdControlSingleton.cpp src/kbd/KbdControlSingleton.h src/audio/AudioUtils.h src/settings/Settings.cpp src/settings/Settings.h src/atkacpi/FanDef.h src/atkacpi/FanDef.cpp)
set(UI src/ui/MainWindow.ui src/kbd/KbdListenerThread.cpp src/kbd/KbdListenerThread.h src/settings/Settings.cpp src/settings/Settings.h)

add_executable(G14ControlPP ${SOURCES} ${UI})
#target_link_libraries(GraphiCa Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(G14ControlPP Qt5::Widgets)
