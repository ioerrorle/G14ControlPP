!WIN32EVENTS_PRI{
    CONFIG += WIN32EVENTS_PRI

    include($$PWD/../Logger/Logger.pri)

    HEADERS += \
            $$PWD/win32eventcontroller.h \
            $$PWD/provider.h

    SOURCES += \
            $$PWD/win32eventcontroller.cpp

    win32 {
        LIBS += -L"advapi32.dll"
    }
}
