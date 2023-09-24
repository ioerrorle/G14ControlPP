TEMPLATE = app
CONFIG   += console qt
QT -= gui
QT += core network

CONFIG += c++17 console
QMAKE_CXXFLAGS *= -std=c++17
#CONFIG -= app_bundle
CONFIG += object_parallel_to_source

INCLUDEPATH += $$clean_path($$PWD/..)

CONFIG += DEVELOPING
!DEVELOPING {
include($$PWD/../3rdParty/qtservice/src/qtservice.pri)

SOURCES += main.cpp
} else {
SOURCES += main_emu.cpp
}
include($$PWD/../Win32Events/Win32Events.pri)
include($$PWD/../G14AcpiController/G14AcpiController.pri)
include($$PWD/../Rpc/server/RpcServer.pri)

HEADERS += \
    $$PWD/controller/servicecontroller.h

SOURCES += \
    $$PWD/controller/servicecontroller.cpp
