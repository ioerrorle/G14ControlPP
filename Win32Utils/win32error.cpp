#include "win32error.h"

void Win32Utils::GetLastErrorAsString(const DWORD errorCode, QString &error)
{
    //Get the error message ID, if any.
    if (errorCode == 0) {
        error = "Error code 0";
        return;
    }

    LPWSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    error = QString::fromWCharArray(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);
}
