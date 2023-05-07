#ifndef WIN32EVENTCONTROLLER_H
#define WIN32EVENTCONTROLLER_H

#include <windows.h>
#include <QString>
#include <Logger/ilogger.h>
#include "provider.h"


class Win32EventController : public ILogger
{
public:
    Win32EventController();
    ~Win32EventController();
    bool init(QString &error);

    void logMessage(QtMsgType type, const QMessageLogContext &, const QString &msg) override;
private:
    constexpr static const LPCWSTR PROVIDER_NAME = L"G14CppService";
    HANDLE m_eventSourceHandle = NULL;

    void deregisterEventSource();    
};

#endif //WIN32EVENTCONTROLLER_H
