#ifndef G14ACPI_H
#define G14ACPI_H

#include <QObject>
#include <windows.h>
#include <fileapi.h>

#include "acpilistenerthread.h"
#include "controlcodes.h"

class G14Acpi : public QObject
{
    Q_OBJECT
public:
    explicit G14Acpi(QObject *parent = nullptr);
    ~G14Acpi();

    bool initAtkAcpi(QString &error);

    bool sendDeviceIoControl(const G14AcpiCodes::Function func,
                             uint32_t arg,
                             QByteArray &result,
                             QString &error);
    bool sendDeviceIoControl(const G14AcpiCodes::Function func,
                             const G14AcpiCodes::CommandCode command,
                             QByteArray &result,
                             QString &error);
    bool sendDeviceIoControl(const G14AcpiCodes::Function func,
                             const G14AcpiCodes::CommandCode command,
                             const uint32_t arg,
                             QByteArray &result,
                             QString &error);
    bool sendDeviceIoControl(const G14AcpiCodes::Function func,
                             const QByteArray &data,
                             QByteArray &result,
                             QString &error);

protected:
    HANDLE m_atkAcpiHandle = nullptr;
    AcpiListenerThread *m_acpiListenerThread = nullptr;

    bool initAcpiListenerThread(QString &error);

signals:
    void acpiEvent(const QByteArray &data);
    void acpiEventError(const QString &error);

};

#endif // G14ACPI_H
