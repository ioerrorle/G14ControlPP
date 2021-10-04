#include "AcpiListenerThread.h"

AcpiListenerThread::AcpiListenerThread(HANDLE acpiHandle, QString &error) {
    this->acpiHandle = acpiHandle;

    unsigned char outBuffer[8];
    DWORD bytesReturned;

    unsigned char initData[12] = "INIT\x04";
    memset(&initData[5], 0, 7);

    WINBOOL result = DeviceIoControl(acpiHandle,
                                     0x22240C,
                                     &initData[0],
                                     8,
                                     &outBuffer[0],
                                     8,
                                     &bytesReturned,
                                     NULL);

    //todo check result

    if (!result) {
        auto errorCode = GetLastError();
    }

    this->eventHandle = CreateEvent(NULL, FALSE, FALSE, "ATK 0100 Sync Event");
}

void AcpiListenerThread::run() {
    unsigned char outBuffer[16];
    DWORD bytesReturned;

    unsigned char data[8] = {};
    data[0] = ((unsigned char *)&this->eventHandle)[0];
    data[1] = ((unsigned char *)&this->eventHandle)[1];
    memset(&data[3], 0, 5);

    WINBOOL result = DeviceIoControl(acpiHandle,
                                     0x222400,
                                     &data[0],
                                     8,
                                     &outBuffer[0],
                                     16,
                                     &bytesReturned,
                                     NULL);

    //todo check result

    auto errorCode = GetLastError();

    //todo check result

    result = WaitForSingleObject(eventHandle, INFINITE);

    //todo check result

    unsigned long emittable = ((unsigned long *)&outBuffer)[0];
    emit resultReady(emittable);

    while(!isFinished()) {
        result = DeviceIoControl(acpiHandle,
                                 0x222408, //0x22240c
                                 NULL, //DSTS\x04 00 00 00 + 4 bytes of device id (00130011 or 00140011)
                                 0, //0c
                                 &outBuffer[0], //read it
                                 4, //read it
                                 &bytesReturned,
                                 NULL);

        //todo check result

        result = WaitForSingleObject(eventHandle, INFINITE);

        //todo check result

        emittable = ((unsigned long *)&outBuffer)[0];
        emit resultReady(emittable);
    }


}


