!ACPIRPC_PRI
{
CONFIG += ACPIRPC_PRI

include($$PWD/../CpuProfile/CpuProfile.pri)

HEADERS += \
        $$PWD/rpcapi.h

SOURCES += \
        $$PWD/rpcapi.cpp
}
