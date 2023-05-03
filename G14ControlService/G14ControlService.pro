TEMPLATE = app
CONFIG   += console qt
QT = core network

INCLUDEPATH += $$clean_path($$PWD/..)

include($$PWD/../3rdParty/qtservice/src/qtservice.pri)
include($$PWD/../RyzenController/RyzenController.pri)
include($$PWD/../G14AcpiController/G14AcpiController.pri)

HEADERS += \
    $$PWD/controller/servicecontroller.h \
    $$PWD/appState/appstate.h

SOURCES += main.cpp \
    $$PWD/controller/servicecontroller.cpp \
    $$PWD/appState/appstate.cpp
