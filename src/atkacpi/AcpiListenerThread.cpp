#include "AcpiListenerThread.h"

AcpiListenerThread::AcpiListenerThread(QString &error) {
    this->eventHandle = CreateEvent(NULL, FALSE, FALSE, "ATK 0100 Sync Event");
}

void AcpiListenerThread::run() {
    unsigned char outBuffer[16];
    DWORD bytesReturned;

    unsigned char data[8] = {};
    data[0] = ((unsigned char *)&this->eventHandle)[0];
    data[1] = ((unsigned char *)&this->eventHandle)[1];
    memset(&data[3], 0, 5);

    AcpiControlSingleton::getInstance().controlInternal(
                                     0x222400,
                                     &data[0],
                                     8,
                                     &outBuffer[0],
                                     16);

    //todo check result

    //auto errorCode = GetLastError();

    //todo check result

    WINBOOL result = WaitForSingleObject(eventHandle, INFINITE);

    //todo check result

    unsigned long emittable = ((unsigned long *)&outBuffer)[0];
    emit resultReady(emittable);

    while(!isFinished()) {
        AcpiControlSingleton::getInstance().controlInternal(
        //result = DeviceIoControl(acpiHandle,
                                 0x222408, //0x22240c
                                 NULL, //DSTS\x04 00 00 00 + 4 bytes of device id (00130011 or 00140011)
                                 0, //0c
                                 &outBuffer[0], //read it
                                 4);
        //todo check result

        result = WaitForSingleObject(eventHandle, INFINITE);

        //todo check result

        emittable = ((unsigned long *)&outBuffer)[0];
        emit resultReady(emittable);
    }


}


