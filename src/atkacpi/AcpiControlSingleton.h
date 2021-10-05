#ifndef G14CONTROLPP_ACPICONTROLSINGLETON_H
#define G14CONTROLPP_ACPICONTROLSINGLETON_H

#include <QObject>
#include "windows.h"
#include <QByteArray>
#include <QtCore/QByteArray>
#include "AcpiListenerThread.h"
#include "FanDef.h"

class AcpiControlSingleton : public QObject {
Q_OBJECT

public:
    static AcpiControlSingleton &getInstance();

    static void fixFanCurve(FAN_DEVICE fanDevice, FanCurve &fanCurve);

private:
    AcpiControlSingleton();

    HANDLE ATKACPIhandle;
    AcpiListenerThread *acpiListenerThread;

    unsigned long
    controlInternal(unsigned long controlCode, unsigned char *inBuffer, int inBufferSize, unsigned char *outBuffer,
                    int outBufferSize);

    void setFanCurve(FAN_DEVICE fanDevice, const FanCurve &fanCurve);

public:
    AcpiControlSingleton(AcpiControlSingleton const &) = delete;

    void operator=(AcpiControlSingleton const &) = delete;

    bool init(QString &error);

    long getCpuFanSpeed();

    long getGpuFanSpeed();
    //quint64 getCpuVoltage();

    int dispose();

    void lcdLightChange(bool increase);

    void setPowerPlan(ASUS_PLAN plan);

    void setFanProfile(const FansProfile &fansProfile);

public slots:
    void handleAcpiEvent(const unsigned long acpiCode);

signals:
    void acpiEvent(const unsigned long acpiCode);
};


#endif //G14CONTROLPP_ACPICONTROLSINGLETON_H
