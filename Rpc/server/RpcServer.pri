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

HEADERS += \
    $$PWD/socketworker.h

SOURCES += \
    $$PWD/socketworker.cpp

