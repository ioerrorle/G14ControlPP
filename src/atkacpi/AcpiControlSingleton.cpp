#include "AcpiControlSingleton.h"

AcpiControlSingleton &AcpiControlSingleton::getInstance() {
    static AcpiControlSingleton instance;
    return instance;
}

AcpiControlSingleton::AcpiControlSingleton() {

}

bool AcpiControlSingleton::init(QString &error) {
    ATKACPIhandle = CreateFile("\\\\.\\ATKACPI",
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,
                               OPEN_EXISTING,
                               0,
                               NULL);

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
    this->acpiListenerThread = new AcpiListenerThread(this->ATKACPIhandle, error);
    connect(this->acpiListenerThread, &AcpiListenerThread::resultReady, this, &AcpiControlSingleton::handleAcpiEvent);
    connect(this->acpiListenerThread, &AcpiListenerThread::finished, this->acpiListenerThread,
            &QObject::deleteLater);
    this->acpiListenerThread->start();

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
    unsigned long bytesReturned;

    WINBOOL result = DeviceIoControl(ATKACPIhandle,
                                     controlCode, //0x22240c
                                     inBuffer, //DSTS\x04 00 00 00 + 4 bytes of device id (00130011 or 00140011)
                                     inBufferSize, //0c
                                     outBuffer, //read it
                                     outBufferSize, //read it
                                     &bytesReturned,
                                     NULL);
    //todo handle result/error

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

    unsigned char input[16] = {0x44, 0x45, 0x56, 0x53, 0x08, 0x00, 0x00, 0x00, /*prefix*/
                               0x21, 0x00, 0x10, 0x00, byte, 0x00, 0x00, 0x00 /* data */
    };

    unsigned char outBuffer[8];

    int bytesWritten = controlInternal(0x22240c, &input[0], 16, &outBuffer[0], 8);
    //todo handle result/error
}

void AcpiControlSingleton::handleAcpiEvent(const unsigned long acpiCode) {
    emit acpiEvent(acpiCode);
}

