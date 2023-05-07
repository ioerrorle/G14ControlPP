!G14ACPICONTROLLER_PRI{
CONFIG += G14ACPICONTROLLER_PRI

include($$PWD/../G14Acpi/G14Acpi.pri)
include($$PWD/../G14ArmouryCrate/G14ArmouryCrate.pri)
include($$PWD/../RyzenController/RyzenController.pri)

HEADERS += \
    $$PWD/g14acpicontroller.h

SOURCES += \
    $$PWD/g14acpicontroller.cpp
}
