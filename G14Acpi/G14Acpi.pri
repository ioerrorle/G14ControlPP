!G14ACPI_PRI{
CONFIG += G14ACPI_PRI

include($$PWD/../Win32Utils/Win32Utils.pri)

HEADERS += \
        $$PWD/g14acpi.h \
        $$PWD/acpilistenerthread.h \
        $$PWD/controlcodes.h

SOURCES += \
        $$PWD/g14acpi.cpp \
        $$PWD/acpilistenerthread.cpp
}
