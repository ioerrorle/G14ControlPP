!JSONHELPER_PRI{
    CONFIG += JSONHELPER_PRI
	
    include($$PWD/../3rdParty/JsonCpp/JsonCpp.pri)

    SOURCES += \
        $$PWD/jsonhelper.cpp

    HEADERS += \
        $$PWD/jsonhelper.h
}


