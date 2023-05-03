!RPCAPI_SERVER_PRI
{
CONFIG += RPCAPI_SERVER_PRI

include($$PWD/../RpcApi.pri)
include($$PWD/../../TcpDaemon/TcpDaemon.pri)

HEADERS += \
        $$PWD/rpcapiserver.h

SOURCES += \
        $$PWD/rpcapiserver.cpp
}
