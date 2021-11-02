#include "AcpiControlSingleton.h"

AcpiControlSingleton &AcpiControlSingleton::getInstance() {
    static AcpiControlSingleton instance;
    return instance;
}

AcpiControlSingleton::AcpiControlSingleton() : mutex() {
    ATKACPIhandle = CreateFile("\\\\.\\ATKACPI",
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,
                               OPEN_EXISTING,
                               0,
                               NULL);
}

bool AcpiControlSingleton::init(QString &error) {
    //todo check if handle was created
    if (!ATKACPIhandle) {
        return false;
    }
    //todo send OSVR (os version)

    //init ACPI
    //todo i think outbuffer doesn't matter, can be 1 byte or so, same goes for inbuffer, 5 bytes is enough i think
    unsigned char outBuffer[8];
    DWORD bytesReturned;

    unsigned char initData[12] = "INIT\x04";
    memset(&initData[5], 0, 7);

    WINBOOL result = DeviceIoControl(ATKACPIhandle,
                                     0x22240C,
                                     &initData[0],
                                     8,
                                     &outBuffer[0],
                                     8,
                                     &bytesReturned,
                                     NULL);

    if (!result) {
        //todo return error string
        auto errorCode = GetLastError();
        return false;
    }

    //todo check outbuffer


    //creating listener thread


    return true;
}

int AcpiControlSingleton::dispose() {
    int result = CloseHandle(ATKACPIhandle);
    if (result != 0) {
        return 1;
    }
    //some error
    return 0;
}

unsigned long AcpiControlSingleton::controlInternal(unsigned long controlCode,
                                                    unsigned char *inBuffer,
                                                    int inBufferSize,
                                                    unsigned char *outBuffer,
                                                    int outBufferSize) {
    //QMutexLocker locker(&mutex);
    //return 0l;
    unsigned long bytesReturned;

    //qDebug() << "start deviceiocontrol";
    mutex.lock();
    WINBOOL result = DeviceIoControl(ATKACPIhandle,
                                     controlCode, //0x22240c
                                     inBuffer, //DSTS\x04 00 00 00 + 4 bytes of device id (00130011 or 00140011)
                                     inBufferSize, //0c
                                     outBuffer, //read it
                                     outBufferSize, //read it
                                     &bytesReturned,
                                     NULL);
    mutex.unlock();
    //todo handle result/error
    //qDebug() << "end deviceiocontrol";
    if (result != FALSE) {
        return bytesReturned;
    }
    //some error
    return 0;
}


long AcpiControlSingleton::getCpuFanSpeed() {
    unsigned char input[12] = {0x44, 0x53, 0x54, 0x53, 0x04, 0x00, 0x00, 0x00, /*prefix*/
                               0x13, 0x00, 0x11, 0x00 /* data */
    };

    unsigned char outBuffer[8];

    int bytesWritten = controlInternal(0x22240c, &input[0], 12, &outBuffer[0], 8);

    if (bytesWritten == 0) {
        return 0;
    }

    long result = (unsigned char) (outBuffer[0]) * 100;

    return result;
}

long AcpiControlSingleton::getGpuFanSpeed() {
    unsigned char input[12] = {0x44, 0x53, 0x54, 0x53, 0x04, 0x00, 0x00, 0x00, /*prefix*/
                               0x14, 0x00, 0x11, 0x00 /* data */
    };

    unsigned char outBuffer[8];

    int bytesWritten = controlInternal(0x22240c, &input[0], 12, &outBuffer[0], 8);

    if (bytesWritten == 0) {
        return 0;
    }

    long result = (unsigned char) (outBuffer[0]) * 100;

    return result;
}

void AcpiControlSingleton::lcdLightChange(bool increase) {
    unsigned char byte = 0x10;
    if (!increase) {
        byte += 0x10;
    }

    unsigned char input[16] = {0x44, 0x45, 0x56, 0x53,
                               0x08, 0x00, 0x00, 0x00, /*prefix*/
                               0x21, 0x00, 0x10, 0x00,
                               byte, 0x00, 0x00, 0x00 /* data */
    };

    unsigned char outBuffer[8];

    int bytesWritten = controlInternal(0x22240c, &input[0], 16, &outBuffer[0], 8);
    //todo handle result/error
}

void AcpiControlSingleton::setPowerPlan(ASUS_PLAN plan) {
    unsigned char input[16] = {0x44, 0x45, 0x56, 0x53,
                               0x08, 0x00, 0x00, 0x00,
                               0x75, 0x00, 0x12, 0x00,
                               plan, 0x00, 0x00, 0x00};

    unsigned char outBuffer[1024];

    int bytesWritten = controlInternal(0x22240c, &input[0], 16, &outBuffer[0], 1024);
}

void AcpiControlSingleton::setFanCurve(FAN_DEVICE fanDevice, const FanCurve &fanCurve) {
    unsigned char input[28] = {0x44, 0x45, 0x56, 0x53,
                               0x14, 0x00, 0x00, 0x00,
                          fanDevice, 0x00, 0x11, 0x00,
                               0xFF, 0xFF, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0xFF};

    //checking fan curve


    /*4 => match device {
                FanCurveDevice::Cpu => 31,
                FanCurveDevice::Gpu => 34,
            },
            5 => match device {
                FanCurveDevice::Cpu => 49,
                FanCurveDevice::Gpu => 51,
            },
            6 | 7 => match device {
                FanCurveDevice::Cpu => 56,
                FanCurveDevice::Gpu => 61,
            },*/

    for (int i = 0; i < 8; i++) {
        //checking fan curve
        uchar minTemp = 30 + 10*i;
        uchar maxTemp = minTemp + 9;

        if (fanCurve.temp[i] < minTemp || fanCurve.temp[i] > maxTemp) {
            return;
        }

        uchar minSpeed = 0;
        switch (i) {
            case 4:
                minSpeed = fanDevice == FAN_CPU ? 31 : 34;
                break;
            case 5:
                minSpeed = fanDevice == FAN_CPU ? 49 : 51;
                break;
            case 6:
            case 7:
                minSpeed = fanDevice == FAN_CPU ? 56 : 61;
                break;
            default:
                break;
        }

        if (fanCurve.speed[i] < minSpeed || fanCurve.speed[i] > 100) {
            return;
        }

        input[12 + i] = fanCurve.temp[i];
        input[12 + i + 8] = fanCurve.speed[i];
    }

    unsigned char outBuffer[1024];

    int bytesWritten = controlInternal(0x22240c, &input[0], 28, &outBuffer[0], 1024);
}

void AcpiControlSingleton::fixFanCurve(FAN_DEVICE fanDevice, FanCurve &fanCurve) {
    for (int i = 0; i < 8; i++) {
        //checking fan curve
        uchar minTemp = 30 + 10*i;
        uchar avgTemp = minTemp + 4;
        uchar maxTemp = minTemp + 9;

        if (fanCurve.temp[i] < minTemp || fanCurve.temp[i] > maxTemp) {
            fanCurve.temp[i] = avgTemp;
        }

        uchar minSpeed = 0;
        switch (i) {
            case 4:
                minSpeed = fanDevice == FAN_CPU ? 31 : 34;
                break;
            case 5:
                minSpeed = fanDevice == FAN_CPU ? 49 : 51;
                break;
            case 6:
            case 7:
                minSpeed = fanDevice == FAN_CPU ? 56 : 61;
                break;
            default:
                break;
        }

        if (fanCurve.speed[i] < minSpeed) {
            fanCurve.speed[i] = minSpeed;
        }

        if (fanCurve.speed[i] > 100) {
            fanCurve.speed[i] = 100;
        }
    }
}

void AcpiControlSingleton::setMaxBatteryPercentage(const uchar val) {
    uchar value = 100;
    if (val < 100) {
        value = val;
    }

    unsigned char input[16] = {0x44, 0x45, 0x56, 0x53,
                               0x08, 0x00, 0x00, 0x00, /*prefix*/
                               0x57, 0x00, 0x12, 0x00,
                              value, 0x00, 0x00, 0x00 /* data */
    };

    unsigned char outBuffer[8];

    int bytesWritten = controlInternal(0x22240c, &input[0], 16, &outBuffer[0], 8);
    //todo handle result/error
}

void AcpiControlSingleton::setFanProfile(const FansProfile &fansProfile) {
    setFanCurve(FAN_CPU, fansProfile.cpu);
    setFanCurve(FAN_GPU, fansProfile.gpu);
}

PowerSourceType AcpiControlSingleton::getPowerSourceType() {
    unsigned char input[12] = {0x44, 0x53, 0x54, 0x53, 0x04, 0x00, 0x00, 0x00, /*prefix*/
                               0x6c, 0x00, 0x12, 0x00 /* data */
    };

    unsigned char outBuffer[4];

    int bytesWritten = controlInternal(0x22240c, &input[0], 12, &outBuffer[0], 4);

    if (bytesWritten == 0) {
        return 0;
    }

    PowerSourceType result = *(PowerSourceType *) (&outBuffer[0]);

    return result;
}

void AcpiControlSingleton::sendSleepCommand() {
    unsigned char input[16] = {0x44, 0x45, 0x56, 0x53,
                               0x08, 0x00, 0x00, 0x00, /*prefix*/
                               0x21, 0x00, 0x10, 0x00,
                               0x6c, 0x00, 0x00, 0x00 /* data */
    };

    unsigned char outBuffer[8];

    int bytesWritten = controlInternal(0x22240c, &input[0], 16, &outBuffer[0], 8);
}

void AcpiControlSingleton::sendRfKillCommand() {
    unsigned char input[16] = {0x44, 0x45, 0x56, 0x53,
                               0x08, 0x00, 0x00, 0x00, /*prefix*/
                               0x21, 0x00, 0x10, 0x00,
                               0x88, 0x00, 0x00, 0x00 /* data */
    };

    unsigned char outBuffer[8];

    int bytesWritten = controlInternal(0x22240c, &input[0], 16, &outBuffer[0], 8);
}

void AcpiControlSingleton::getFanCurves(FAN_DEVICE device, unsigned char plan) {
    unsigned char input[16] = {0x44, 0x53, 0x54, 0x53,
                               0x08, 0x00, 0x00, 0x00,
                               device, 0x00, 0x11, 0x00,
                               plan, 0x00, 0x00, 0x00};

    unsigned char outBuffer[16];

    int bytesWritten = controlInternal(0x22240c, &input[0], 16, &outBuffer[0], 16);

    auto deb = qDebug();
    for(int i = 0; i < 16; i++) {
        deb << outBuffer[i] << ";";
    }
}

