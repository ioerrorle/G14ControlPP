#ifndef G14CONTROLPP_FANCONTROLSINGLETON_H
#define G14CONTROLPP_FANCONTROLSINGLETON_H

#include "windows.h"
#include <QByteArray>
#include <QtCore/QByteArray>

class FanControlSingleton {
public:
    static FanControlSingleton &getInstance();

private:
    FanControlSingleton();

    HANDLE ATKACPIhandle;

    int controlInternal(unsigned long controlCode, unsigned char *inBuffer, int inBufferSize, unsigned char *outBuffer, int outBufferSize);

public:
    FanControlSingleton(FanControlSingleton const &) = delete;

    void operator=(FanControlSingleton const &) = delete;

    bool init();

    long getCpuFanSpeed();
    long getGpuFanSpeed();
    //quint64 getCpuVoltage();

    int dispose();

    long kbdLightPlus();

    long kbdLightMinus();
};


#endif //G14CONTROLPP_FANCONTROLSINGLETON_H
