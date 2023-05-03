#include "g14acpi.h"
#include "Win32Utils/win32error.h"

#include <QVariant>

#include <QDebug>

G14Acpi::G14Acpi(QObject *parent)
    : QObject{parent}
{

}

G14Acpi::~G14Acpi()
{
    if (m_atkAcpiHandle)
        CloseHandle(m_atkAcpiHandle);
}

bool G14Acpi::initAtkAcpi(QString &error)
{
    m_atkAcpiHandle = CreateFile(L"\\\\.\\ATKACPI",
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,
                               OPEN_EXISTING,
                               0,
                               NULL);

    if (m_atkAcpiHandle == INVALID_HANDLE_VALUE) {
        DWORD errorCode = GetLastError();
        Win32Utils::GetLastErrorAsString(errorCode, error);
        m_atkAcpiHandle = nullptr;
        return false;
    }
    //initialize G14 laptop
    /* should be like this:
     * 1) OSVR 0x00040000
     * 2) INIT
     * 3) SFUN (should return 0x21)
     * 4) DSTS 0x51001000 (should return 0x010001) ???
     * 5) WDOG
     * 6) DSTS 0x21000500 - report keyboard backlight capability?
     * 7) DEVS 0x11000500 0x02000000
     * 8) DSTS 0x57001200 - get charge limit
     * 9) DEVS 0x57001200 - set charge limit
     */

    QByteArray result;

    if (!sendDeviceIoControl(G14AcpiCodes::Function::OSVR, 0x400, result, error))
        return false;

    if (!sendDeviceIoControl(G14AcpiCodes::Function::INIT, 0x0, result, error))
        return false;

    //useless
    //if (!sendDeviceIoControl(G14AcpiCodes::Function::SFUN, G14AcpiCodes::CommandCode::NO_CMD, result, error))
    //    return false;

    //useless
    //if (!sendDeviceIoControl(G14AcpiCodes::Function::WDOG, G14AcpiCodes::CommandCode::NO_CMD, result, error))
    //    return false;

    uint32_t unknownCommand1arg = 0x2;

    if (!sendDeviceIoControl(G14AcpiCodes::Function::DEVS, G14AcpiCodes::CommandCode::DEVID_KBD_BACKLIGHT, unknownCommand1arg, result, error))
        return false;

    if (!initAcpiListenerThread(error))
        return false;

    connect(m_acpiListenerThread, &AcpiListenerThread::acpiEvent, this, [=](const QByteArray &data) {
       qDebug() << "onAcpiEvent" << data;
    }, Qt::QueuedConnection);

    connect(m_acpiListenerThread, &AcpiListenerThread::acpiEventError, this, [=](const QString &error) {
       qDebug() << "onAcpiError" << error;
    }, Qt::QueuedConnection);

    return true;
}

bool G14Acpi::initAcpiListenerThread(QString &error) {
    if (!m_atkAcpiHandle) {
        error = "ATK ACPI is not initialized";
        return false;
    }

    m_acpiListenerThread = new AcpiListenerThread(m_atkAcpiHandle);
    if (!m_acpiListenerThread->init(error))
        return false;

    connect(m_acpiListenerThread, &AcpiListenerThread::acpiEvent, this, [=](const QByteArray &data) {
       emit acpiEvent(data);
    });
    connect(m_acpiListenerThread, &AcpiListenerThread::acpiEventError, this, [=](const QString &error) {
       emit acpiEventError(error);
    });

    m_acpiListenerThread->start();

    return true;
}

bool G14Acpi::sendDeviceIoControl(const G14AcpiCodes::Function func,
                                  uint32_t arg,
                                  QByteArray &result,
                                  QString &error) {
    QByteArray commandBytes = QByteArray::fromRawData((const char *)&arg, sizeof (arg));
    return sendDeviceIoControl(func, commandBytes, result, error);
}

bool G14Acpi::sendDeviceIoControl(const G14AcpiCodes::Function func,
                                  const G14AcpiCodes::CommandCode command,
                                  QByteArray &result,
                                  QString &error)
{
    QByteArray commandBytes = QByteArray::fromRawData((const char *)&command, sizeof (command));

    return sendDeviceIoControl(func, commandBytes, result, error);
}

bool G14Acpi::sendDeviceIoControl(const G14AcpiCodes::Function func,
                                  const G14AcpiCodes::CommandCode command,
                                  const uint32_t arg,
                                  QByteArray &result,
                                  QString &error)
{
    QByteArray commandBytes = QByteArray::fromRawData((const char *)&command, sizeof (command));
    QByteArray argBytes = QByteArray::fromRawData((const char *)&arg, sizeof (arg));
    commandBytes.append(argBytes);

    return sendDeviceIoControl(func, commandBytes, result, error);
}

bool G14Acpi::sendDeviceIoControl(const G14AcpiCodes::Function function,
                                  const QByteArray &data,
                                  QByteArray &result,
                                  QString &error)
{
    /* syntax:
     * 4 bytes of command
     * 4 bytes of command size
     * n bytes of command itself
     */

    result.fill(0);

    if (!m_atkAcpiHandle) {
        error = "ATK ACPI is not initialized";
        return false;
    }

    QByteArray dataBytes = QByteArray::fromRawData((const char *)&function, sizeof (function));
    if (dataBytes.size() != 4) {
        error = "Command size is not 4";
        return false;
    }
    uint32_t commandSize = data.size();

    dataBytes.append(QByteArray::fromRawData((const char *)&commandSize, sizeof (commandSize)));
    dataBytes.append(data);

    int outDataSize = std::max(0x0c, data.size());
    char outData[outDataSize];

    ulong bytesReturned = 0;

    WINBOOL deviceIoControlResult = DeviceIoControl(m_atkAcpiHandle,
                                     (DWORD)G14AcpiCodes::ControlCode::ATK_ACPI,
                                     (LPVOID)dataBytes.constData(),
                                     (DWORD)dataBytes.size(),
                                     (LPVOID)outData,
                                     (DWORD)outDataSize,
                                     (LPDWORD)&bytesReturned,
                                     NULL);

    if (!deviceIoControlResult) {
        DWORD lastError = GetLastError();
        QString lastErrorString = "Unknown";
        Win32Utils::GetLastErrorAsString(lastError, lastErrorString);
        error = QString("[ACPI controller] Device control error: %1").arg(lastErrorString);
        return false;
    }


    int size = std::min(result.size(), outDataSize);
    result.replace(0, size, outData, size);
    size = result.size();

    return true;
}
