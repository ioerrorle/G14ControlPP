!RPCCLIENT_PRI
{
    CONFIG += RPCCLIENT_PRI

    include($$PWD/../proto/RpcProtocol.pri)

}

HEADERS += \
    $$PWD/rpcclient.h \
    $$PWD/tcpclient.h

SOURCES += \
    $$PWD/rpcclient.cpp \
    $$PWD/tcpclient.cpp
