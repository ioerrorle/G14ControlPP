!RPCPROTOCOL_PRI{
CONFIG += RPCPROTOCOL_PRI

include($$PWD/../../JsonHelper/JsonHelper.pri)
include($$PWD/../../G14AppState/AppState.pri)

HEADERS += \
    $$PWD/request/appstaterequest.h \
    $$PWD/request/baserequest.h \
    $$PWD/response/appstateresponse.h \
    $$PWD/response/baseresponse.h \
    $$PWD/response/error.h \
    $$PWD/rpcmessagetype.h

SOURCES += \
    $$PWD/request/appstaterequest.cpp \
    $$PWD/response/appstateresponse.cpp \
    $$PWD/response/error.cpp \
    $$PWD/rpcmessagetype.cpp

}
