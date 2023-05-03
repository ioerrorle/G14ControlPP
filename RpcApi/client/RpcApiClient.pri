!RPCAPI_CLIENT_PRI
{
CONFIG += RPCAPI_CLIENT_PRI

include($$PWD/../RpcApi.pri)

HEADERS += \
		$$PWD/rpcapiclient.h

SOURCES += \
        $$PWD/rpcapiclient.cpp
}
