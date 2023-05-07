#include "win32eventcontroller.h"

Win32EventController::Win32EventController()
{

}

Win32EventController::~Win32EventController()
{
    deregisterEventSource();
}

bool Win32EventController::init(QString &error)
{
    m_eventSourceHandle = RegisterEventSource(NULL, PROVIDER_NAME);
    if (NULL == m_eventSourceHandle)
    {
        return false;
    }

    return true;
}

void Win32EventController::logMessage(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    WORD win32EventType = 0;
    switch (type) {
        case QtMsgType::QtWarningMsg:
            win32EventType = EVENTLOG_WARNING_TYPE;
            break;
        case QtMsgType::QtCriticalMsg:
            win32EventType = EVENTLOG_ERROR_TYPE;
            break;
        default:
            win32EventType = EVENTLOG_INFORMATION_TYPE;
            break;
    }

    LPCWSTR messageParams[] = { (LPCWSTR)msg.utf16() };
    ReportEvent(m_eventSourceHandle, win32EventType, 0, GENERAL_EVENT, NULL, 1, 0, messageParams, NULL);
}

void Win32EventController::deregisterEventSource()
{
    if (NULL != m_eventSourceHandle)
        DeregisterEventSource(m_eventSourceHandle);
}
