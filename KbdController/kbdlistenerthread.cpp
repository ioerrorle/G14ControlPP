#include "kbdlistenerthread.h"
#include "Win32Utils/win32error.h"
#include <QDebug>
#include <QEvent>

KbdListenerThread::KbdListenerThread(QObject *parent)
    : QThread{parent}
{

}

KbdListenerThread::~KbdListenerThread()
{
    if (m_kbdEventHandle)
        CloseHandle(m_kbdEventHandle);
    if (m_quitEventHandle)
        CloseHandle(m_quitEventHandle);
}

bool KbdListenerThread::init(HANDLE kbdHandle, QString &error)
{
    m_kbdHandle = kbdHandle;
    m_kbdEventHandle = CreateEvent(NULL, FALSE, FALSE, L"KbdEvent");
    if (m_kbdEventHandle == NULL) {
        DWORD lastError = GetLastError();
        QString lastErrorString = "Unknown";
        Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
        error = QString("[KBD thread] Can't create event handle: %1").arg(lastErrorString);
        return false;
    }

    m_quitEventHandle = CreateEvent(NULL, FALSE, FALSE, L"KbdQuitEvent");
    if (m_quitEventHandle == NULL) {
        DWORD lastError = GetLastError();
        QString lastErrorString = "Unknown";
        Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
        error = QString("[KBD thread] Can't create quit event handle: %1").arg(lastErrorString);
        return false;
    }

    return true;
}

void KbdListenerThread::quit()
{
    qDebug() << "Quit called!";
    if (m_quitEventHandle)
        SetEvent(m_quitEventHandle);

    QThread::quit();
}

void KbdListenerThread::run()
{
    QString error;
    int lastError = 0;

    uchar kbdReadResult[6];
    DWORD bytesRead;

    OVERLAPPED kbdOverlap = {};
    kbdOverlap.hEvent = m_kbdEventHandle;

    size_t handleCount = 2;
    int quitHandleIndex = 0;
    HANDLE handles[] = {m_quitEventHandle, m_kbdEventHandle};

    while (!isFinished()) {
        WINBOOL readResult = ReadFile(m_kbdHandle,
                                      kbdReadResult,
                                      6,
                                      &bytesRead,
                                      &kbdOverlap);
        lastError = GetLastError();
        if (!readResult && lastError != ERROR_IO_PENDING) {
            DWORD lastError = GetLastError();
            QString lastErrorString = "Unknown";
            Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
            emit kbdError(QString("[KBD thread] Device doesn't support defined reading: %1").arg(lastErrorString));
            break;
        }

        DWORD waitResult = WaitForMultipleObjects(handleCount, handles, false, INFINITE);
        if (waitResult == WAIT_TIMEOUT) {
            emit kbdError("[KBD thread] Wait timeout");
        } else if (waitResult == WAIT_FAILED) {
            DWORD lastError = GetLastError();
            QString lastErrorString = "Unknown";
            Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
            emit kbdError(QString("[KBD thread] Wait error: %1").arg(lastErrorString));
        } else if (waitResult >= WAIT_ABANDONED_0 && waitResult <= WAIT_ABANDONED_0 - handleCount) {
            int handleIndex = waitResult - WAIT_ABANDONED_0;
            emit kbdError(QString("[KBD thread] Wait for handle %1 abandoned").arg(handleIndex));
        } else {
            int handleIndex = waitResult - WAIT_OBJECT_0;
            if (handleIndex == quitHandleIndex) { //means exit thread event
                break;
            } else {
                emitKeyPress(kbdReadResult, 6);
            }
        }

        kbdOverlap = {};
        kbdOverlap.hEvent = m_kbdEventHandle;
    }
}

void KbdListenerThread::emitKeyPress(const uchar *cc, int nCount)
{
    if (nCount < 2) {
        return;
        //looks like it's not what we are looking for
    }
    if (cc[1] == 0xec) {
        //it's Fn press or something like that, returning.
        return;
    }

    emit keyPressed(cc[1]);
}
