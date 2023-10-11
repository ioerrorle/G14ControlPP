!G14APPSTATE_PRI{
CONFIG += G14APPSTATE_PRI

include($$PWD/../G14ArmouryCrate/G14ArmouryCrate.pri)
include($$PWD/../CpuProfile/CpuProfile.pri)
include($$PWD/../PowerSource/PowerSource.pri)
include($$PWD/../FanCurve/FanCurve.pri)

HEADERS += \
        $$PWD/appstate.h

SOURCES += \
        $$PWD/appstate.cpp
}
