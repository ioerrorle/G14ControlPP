!RYZENADJ_PRI {
    CONFIG += RYZENADJ_PRI

    INCLUDEPATH *= $$clean_path($$PWD/win32/include)

    win32 {
        LIBS += -L"$$PWD/win32/lib" -lryzenadj
    }
}
