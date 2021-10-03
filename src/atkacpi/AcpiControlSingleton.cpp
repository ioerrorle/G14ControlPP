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
    if (!ATKACPIhandle) {
        return false;
    }

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
}



