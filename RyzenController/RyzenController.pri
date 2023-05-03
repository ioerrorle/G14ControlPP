!RYZENCONTROLLER_PRI
{
    CONFIG += RYZENCONTROLLER_PRI

    include($$PWD/../CpuProfile/CpuProfile.pri)
    include($$PWD/../3rdParty/RyzenAdj/RyzenAdj.pri)

    HEADERS += \
        $$PWD/ryzencontroller.h

    SOURCES += \
        $$PWD/ryzencontroller.cpp
}
