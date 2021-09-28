#include "FanControlSingleton.h"

FanControlSingleton &FanControlSingleton::getInstance() {
    static FanControlSingleton instance;
    return instance;
}

FanControlSingleton::FanControlSingleton() {

}

bool FanControlSingleton::init() {
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

int FanControlSingleton::dispose() {
    int result = CloseHandle(ATKACPIhandle);
    if (result != 0) {
        return 1;
    }
    //some error
    return 0;
}

int FanControlSingleton::controlInternal(unsigned long controlCode, unsigned char *inBuffer, int inBufferSize,
                                         unsigned char *outBuffer, int outBufferSize) {
    unsigned long bytesReturned;

    int result = DeviceIoControl(ATKACPIhandle,
                                 controlCode, //0x22240c
                                 inBuffer, //DSTS\x04 00 00 00 + 4 bytes of device id (00130011 or 00140011)
                                 inBufferSize, //0c
                                 outBuffer, //read it
                                 outBufferSize, //read it
                                 &bytesReturned,
                                 NULL);

    if (result != 0) {
        return bytesReturned;
    }
    //some error
    return 0;
}


long FanControlSingleton::getCpuFanSpeed() {
    unsigned char *input = new unsigned char[12]{0x44, 0x53, 0x54, 0x53, 0x04, 0x00, 0x00, 0x00, /*prefix*/
                               0x13, 0x00, 0x11, 0x00 /* data */
    };

    unsigned char *outBuffer = new unsigned char[8];

    int bytesWritten = controlInternal(0x22240c, input, 12, outBuffer, 8);

    if (bytesWritten == 0) {
        return 0;
    }

    long result = (unsigned char) (outBuffer[0]) * 100;

    return result;
}

long FanControlSingleton::getGpuFanSpeed() {
    unsigned char *input = new unsigned char[12]{0x44, 0x53, 0x54, 0x53, 0x04, 0x00, 0x00, 0x00, /*prefix*/
                               0x14, 0x00, 0x11, 0x00 /* data */
    };

    unsigned char *outBuffer = new unsigned char[8];

    int bytesWritten = controlInternal(0x22240c, input, 12, outBuffer, 8);

    if (bytesWritten == 0) {
        return 0;
    }

    long result = (unsigned char) (outBuffer[0]) * 100;

    return result;
}

long FanControlSingleton::kbdLightPlus() {
    unsigned char *input = new unsigned char[16]{0x44, 0x45, 0x56, 0x53, 0x08, 0x00, 0x00, 0x00, /*prefix*/
                               0x21, 0x00, 0x10, 0x00, 0xC4, 0x00, 0x00, 0x00 /* data */
    };

    unsigned char *outBuffer = new unsigned char[8];

    int bytesWritten = controlInternal(0x22240c, input, 16, outBuffer, 8);

    if (bytesWritten == 0) {
        return 0;
    }

    long result = (unsigned char) (outBuffer[0]) * 100;

    return result;
}

long FanControlSingleton::kbdLightMinus() {
    unsigned char *input = new unsigned char[16]{0x44, 0x45, 0x56, 0x53, 0x08, 0x00, 0x00, 0x00, /*prefix*/
                                                 0x21, 0x00, 0x10, 0x00, 0xC5, 0x00, 0x00, 0x00 /* data */
    };

    unsigned char *outBuffer = new unsigned char[8];

    int bytesWritten = controlInternal(0x22240c, input, 16, outBuffer, 8);

    if (bytesWritten == 0) {
        return 0;
    }

    long result = (unsigned char) (outBuffer[0]) * 100;

    return result;
}

//quint64 FanControlSingleton::getCpuVoltage() {
//    char *input = new char[12]{0x44, 0x53, 0x54, 0x53, 0x04, 0x00, 0x00, 0x00, /*prefix*/
//                               0x79, 0x00, 0x12, 0x00 /* data */
//    };
//
//    unsigned char *outBuffer = new unsigned char[8];
//
//    int bytesWritten = controlInternal(0x22240c, input, 12, outBuffer, 8);
//
//    if (bytesWritten == 0) {
//        return 0;
//    }
//
//    quint64 result = quint64((unsigned char) (outBuffer[0]) << 56 |
//                             (unsigned char) (outBuffer[1]) << 48 |
//                             (unsigned char) (outBuffer[2]) << 40 |
//                             (unsigned char) (outBuffer[3]) << 32 |
//                             (unsigned char) (outBuffer[4]) << 24 |
//                             (unsigned char) (outBuffer[5]) << 16 |
//                             (unsigned char) (outBuffer[6]) << 8 |
//                             (unsigned char) (outBuffer[7]));
//
//    return result;
//}


