#ifndef G14CONTROLPP_ACPICONTROLSINGLETON_H
#define G14CONTROLPP_ACPICONTROLSINGLETON_H

#include <QObject>
#include <QMutex>
#include "windows.h"
#include <QByteArray>
#include <QtCore/QByteArray>
#include <QDebug>
#include <src/model/ArmouryCratePlan.h>
#include <src/model/FanCurve.h>
#include <src/model/FansProfile.h>

typedef long PowerSourceType;

#define POWER_SOURCE_USB 0x00010002
#define POWER_SOURCE_180W 0x00010001
#define POWER_SOURCE_BATTERY 0x00000000

class AcpiControlSingleton : public QObject {
Q_OBJECT

public:
    static AcpiControlSingleton &getInstance();

    static void fixFanCurve(FAN_DEVICE fanDevice, const FanCurve &fanCurve);

private:
    AcpiControlSingleton();

    HANDLE ATKACPIhandle;
    QMutex mutex;

    void setFanCurve(FAN_DEVICE fanDevice, const FanCurve &fanCurve);

public:
    AcpiControlSingleton(AcpiControlSingleton const &) = delete;

    void operator=(AcpiControlSingleton const &) = delete;

    bool init(QString &error);

    unsigned long
    controlInternal(unsigned long controlCode, unsigned char *inBuffer, int inBufferSize, unsigned char *outBuffer,
                    int outBufferSize);

    long getCpuFanSpeed();

    long getGpuFanSpeed();



    int dispose();

    void lcdLightChange(bool increase);

    void setPowerPlan(ASUS_PLAN plan);

    void setFanProfile(const FansProfile &fansProfile);

    void setMaxBatteryPercentage(const uchar value);

    PowerSourceType getPowerSourceType();

    void sendSleepCommand();

    void sendRfKillCommand();

    void getFanCurves(FAN_DEVICE device, unsigned char plan);
};


#endif //G14CONTROLPP_ACPICONTROLSINGLETON_H
