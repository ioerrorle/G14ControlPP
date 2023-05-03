!RYZENADJ_PRI
{
    CONFIG += RYZENADJ_PRI

    win32 {
        INCLUDEPATH *= $$clean_path($$PWD/win32/lib)
        INCLUDEPATH *= $$clean_path($$PWD/win32/include)

        LIBS += -L"$$PWD/win32/lib" -lryzenadj
    }
}
