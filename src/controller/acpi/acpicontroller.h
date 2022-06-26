#ifndef ACPICONTROLLER_H
#define ACPICONTROLLER_H

#include "windows.h"
#include <src/model/ArmouryCratePlan.h>
#include <src/model/FanCurve.h>
#include <src/model/FansProfile.h>
#include <QByteArray>
#include <QDebug>
#include <QMutex>
#include <QObject>
#include <QtCore/QByteArray>

typedef long PowerSourceType;

#define POWER_SOURCE_USB 0x00010002
#define POWER_SOURCE_180W 0x00010001
#define POWER_SOURCE_BATTERY 0x00000000

class AcpiController : public QObject {
    Q_OBJECT

public:
    AcpiController();

private:
    HANDLE ATKACPIhandle;
    QMutex mutex;

    void setFanCurve(FAN_DEVICE fanDevice, const FanCurve &fanCurve);

    unsigned long executeAcpiCommand(unsigned long controlCode,
                                  unsigned char *inBuffer,
                                  int inBufferSize,
                                  unsigned char *outBuffer,
                                  int outBufferSize);

public:
    AcpiController(AcpiController const &) = delete;

    void operator=(AcpiController const &) = delete;

    bool init(QString &error);

    long getCpuFanSpeed();

    long getGpuFanSpeed();

    bool dispose(QString &error);

    void lcdLightChange(bool increase);

    void setPowerPlan(ASUS_PLAN plan);

    void setFanProfile(const FansProfile &fansProfile);

    void setMaxBatteryPercentage(const uchar value);

    PowerSourceType getPowerSourceType();

    void sendSleepCommand();

    void sendRfKillCommand();

    void getFanCurves(FAN_DEVICE device, unsigned char plan);

public slots:
    void onAtkAcpiCommandRequested(unsigned long controlCode,
                                  unsigned char *inBuffer,
                                  int inBufferSize,
                                  unsigned char *outBuffer,
                                  int outBufferSize);
    void onAcpiEvent(const unsigned long acpiCode);

signals:
    void acpiEvent(const unsigned long acpiCode);



};

#endif // ACPICONTROLLER_H
