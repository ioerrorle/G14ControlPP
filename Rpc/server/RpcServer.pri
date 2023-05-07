!RPCSERVER_PRI{
CONFIG += RPCSERVER_PRI

include($$PWD/../proto/RpcProtocol.pri)

HEADERS += \
    $$PWD/rpcserver.h \
    $$PWD/rpcservercontroller.h

SOURCES += \
    $$PWD/rpcserver.cpp \
    $$PWD/rpcservercontroller.cpp

}

