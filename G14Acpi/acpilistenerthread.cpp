#include "acpilistenerthread.h"
#include "controlcodes.h"
#include "Win32Utils/win32error.h"
#include <synchapi.h>
#include <QDebug>

AcpiListenerThread::AcpiListenerThread(const HANDLE atkAcpiHandle, QObject *parent)
    : QThread{parent}
    , m_atkAcpiHandle(atkAcpiHandle)
{

}

AcpiListenerThread::~AcpiListenerThread()
{
    if (m_eventHandle)
        CloseHandle(m_eventHandle);
    if (m_quitEventHandle)
        CloseHandle(m_quitEventHandle);
}

bool AcpiListenerThread::init(QString &error) {
    m_eventHandle = CreateEvent(NULL, FALSE, FALSE, L"ATK 0100 Sync Event");
    if (!m_eventHandle) {
        DWORD lastError = GetLastError();
        QString lastErrorString = "Unknown";
        Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
        error = QString("[ACPI thread] Can't create event handle: %1").arg(lastErrorString);
        return false;
    }

    m_quitEventHandle = CreateEvent(NULL, FALSE, FALSE, L"ATK 0100 Exit Event");
    if (!m_quitEventHandle) {
        DWORD lastError = GetLastError();
        QString lastErrorString = "Unknown";
        Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
        error = QString("[ACPI thread] Can't create exit event handle: %1").arg(lastErrorString);
        return false;
    }
    return true;
}

void AcpiListenerThread::run() {
    size_t outBufferSize = 16;
    char outBuffer[outBufferSize];
    ulong bytesReturned = 0;

    DWORD handleCount = 2;
    HANDLE handles[] = {m_quitEventHandle, m_eventHandle};

    auto value = std::addressof(m_eventHandle);
    auto valueSize = sizeof(value);

    WINBOOL deviceIoControlResult = DeviceIoControl(m_atkAcpiHandle,
                                     (DWORD)G14AcpiCodes::ControlCode::FIRST_EVENT_SUBSCRIBE,
                                     value,
                                     valueSize,
                                     outBuffer,
                                     outBufferSize,
                                     &bytesReturned,
                                     NULL);

    if (!deviceIoControlResult) {
        DWORD lastError = GetLastError();
        QString lastErrorString = "Unknown";
        Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
        emit acpiEventError(QString("[ACPI thread] Device control error: %1").arg(lastErrorString));
    } else {
        DWORD waitResult = WaitForMultipleObjects(handleCount, handles, FALSE, INFINITE);
        bool shouldExit = false;
        bool shouldEmitResult = processWaitResults(waitResult, handleCount, 0, shouldExit);
        if (shouldExit)
            return;
        if (shouldEmitResult) {
            emit acpiEvent(QByteArray(outBuffer, bytesReturned));
        }
    }

    while(!isFinished()) {
        deviceIoControlResult = DeviceIoControl(m_atkAcpiHandle,
                                         (DWORD)G14AcpiCodes::ControlCode::EVENT_SUBSCRIBE,
                                         nullptr,
                                         0,
                                         outBuffer,
                                         outBufferSize,
                                         &bytesReturned,
                                         NULL);

        if (!deviceIoControlResult) {
            DWORD lastError = GetLastError();
            QString lastErrorString = "Unknown";
            Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
            emit acpiEventError(QString("[ACPI thread] Device control error: %1").arg(lastErrorString));
        } else {
            DWORD waitResult = WaitForMultipleObjects(handleCount, handles, FALSE, INFINITE);
            bool shouldExit = false;
            bool shouldEmitResult = processWaitResults(waitResult, handleCount, 0, shouldExit);
            if (shouldExit)
                return;
            if (shouldEmitResult) {
                emit acpiEvent(QByteArray(outBuffer, bytesReturned));
            }
        }
    }
}

bool AcpiListenerThread::processWaitResults(const DWORD waitResult, const DWORD handleCount, const int exitHandleIndex, bool &shouldExit)
{
    shouldExit = false;
    if (waitResult == WAIT_TIMEOUT) {
        emit acpiEventError("[ACPI thread] Wait timeout");
    } else if (waitResult == WAIT_FAILED) {
        DWORD lastError = GetLastError();
        QString lastErrorString = "Unknown";
        Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
        emit acpiEventError(QString("[ACPI thread] Wait error: %1").arg(lastErrorString));
    } else if (waitResult >= WAIT_ABANDONED_0 && waitResult <= WAIT_ABANDONED_0 - handleCount) {
        int handleIndex = waitResult - WAIT_ABANDONED_0;
        emit acpiEventError(QString("[ACPI thread] Wait for handle %1 abandoned").arg(handleIndex));
    } else {
        int handleIndex = waitResult - WAIT_OBJECT_0;
        if (handleIndex == exitHandleIndex) { //means exit thread event
            shouldExit = true;
        } else {
            return true;
        }
    }

    return false;
}
