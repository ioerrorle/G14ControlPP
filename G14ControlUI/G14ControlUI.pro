QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++17
QMAKE_CXXFLAGS *= -std=c++17
#CONFIG -= app_bundle
CONFIG += object_parallel_to_source

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/../G14AppState/AppState.pri)
include($$PWD/../Rpc/client/RpcClient.pri)

INCLUDEPATH += $$clean_path($$PWD/../)

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/ui/mainwindow.cpp \
    $$PWD/ui/powerProfileTab/powerprofiletab.cpp \
    $$PWD/ui/powerProfileTab/arCrate/armourycratewidget.cpp \
    $$PWD/ui/powerProfileTab/cpu/cpuwidget.cpp \
    $$PWD/ui/powerProfileTab/fans/fanswidget.cpp \
    $$PWD/ui/powerProfileTab/fans/fanCurveWidget/edge.cpp \
    $$PWD/ui/powerProfileTab/fans/fanCurveWidget/node.cpp \
    $$PWD/ui/powerProfileTab/fans/fanCurveWidget/fanCurveWidget.cpp \
    controller/maincontroller.cpp

HEADERS += \
    $$PWD/ui/mainwindow.h \
    $$PWD/ui/powerProfileTab/powerprofiletab.h \
    $$PWD/ui/powerProfileTab/arCrate/armourycratewidget.h \
    $$PWD/ui/powerProfileTab/cpu/cpuwidget.h \
    $$PWD/ui/powerProfileTab/fans/fanswidget.h \
    $$PWD/ui/powerProfileTab/fans/fanCurveWidget/edge.h \
    $$PWD/ui/powerProfileTab/fans/fanCurveWidget/node.h \
    $$PWD/ui/powerProfileTab/fans/fanCurveWidget/fanCurveWidget.h \
    controller/maincontroller.h

FORMS += \
    $$PWD/ui/mainwindow.ui \
    $$PWD/ui/powerProfileTab/powerprofiletab.ui \
    $$PWD/ui/powerProfileTab/arCrate/armourycratewidget.ui \
    $$PWD/ui/powerProfileTab/cpu/cpuwidget.ui \
    $$PWD/ui/powerProfileTab/fans/fanswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
