#ifndef WIN32ERROR_H
#define WIN32ERROR_H

#include <windows.h>
#include <QString>

namespace Win32Utils
{
    void GetLastErrorAsString(const DWORD errorCode, QString &error);
}

#endif
