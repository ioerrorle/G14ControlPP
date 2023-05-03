!KBDCONTROLLER_PRI
{
CONFIG += KBDCONTROLLER_PRI

include($$PWD/../Win32Utils/Win32Utils.pri)

HEADERS += \
        $$PWD/hidcontrolcodes.h \
        $$PWD/kbdcontroller.h \
        $$PWD/kbdlistenerthread.h

SOURCES += \
        $$PWD/kbdcontroller.cpp \
        $$PWD/kbdlistenerthread.cpp

LIBS += -lsetupapi
}
