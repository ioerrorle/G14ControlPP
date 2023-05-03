!RPCPROTOCOL_PRI
{
CONFIG += RPCPROTOCOL_PRI

include($$PWD/../../JsonHelper/JsonHelper.pri)
include($$PWD/../../CpuProfile/CpuProfile.pri)

HEADERS += \
    $$PWD/request/baserequest.h \
    $$PWD/request/cpustaterequest.h \
    $$PWD/response/baseresponse.h \
    $$PWD/response/cpustateresponse.h \
    $$PWD/response/error.h \
    $$PWD/rpcmessagetype.h

SOURCES += \
    $$PWD/request/cpustaterequest.cpp \
    $$PWD/response/cpustateresponse.cpp \
    $$PWD/response/error.cpp \
    $$PWD/rpcmessagetype.cpp

}
