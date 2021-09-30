#ifndef G14CONTROLPP_ACPICONTROLSINGLETON_H
#define G14CONTROLPP_ACPICONTROLSINGLETON_H

#include "windows.h"
#include <QByteArray>
#include <QtCore/QByteArray>
#include <ACPIListenerThread.h>

class AcpiControlSingleton {
public:
    static AcpiControlSingleton &getInstance();

private:
    AcpiControlSingleton();

    HANDLE ATKACPIhandle;

    unsigned long controlInternal(unsigned long controlCode, unsigned char *inBuffer, int inBufferSize, unsigned char *outBuffer, int outBufferSize);

public:
    AcpiControlSingleton(AcpiControlSingleton const &) = delete;

    void operator=(AcpiControlSingleton const &) = delete;

    bool init();

    long getCpuFanSpeed();
    long getGpuFanSpeed();
    //quint64 getCpuVoltage();

    int dispose();

    void lcdLightChange(bool increase);
};


#endif //G14CONTROLPP_ACPICONTROLSINGLETON_H
