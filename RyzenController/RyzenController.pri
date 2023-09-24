!RYZENCONTROLLER_PRI{
    CONFIG += RYZENCONTROLLER_PRI

    include($$PWD/../CpuProfile/CpuProfile.pri)
    include($$PWD/../3rdParty/RyzenAdj/RyzenAdj.pri)

    HEADERS += \
        $$PWD/ryzencontroller.h

    !DEVELOPING{
        SOURCES += \
            $$PWD/ryzencontroller.cpp
    } else {
        SOURCES += \
            $$PWD/ryzencontroller_emu.cpp
    }
}
