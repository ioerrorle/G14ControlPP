#include "AcpiListenerThread.h"

AcpiListenerThread::AcpiListenerThread(QString &error) {


    this->acpiHandle = CreateFile("\\\\.\\ATKACPI\\ASLDSRV",
                                                      GENERIC_READ | GENERIC_WRITE,
                                                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                      NULL,
                                                      OPEN_EXISTING,
                                                      0,
                                                      NULL);

    this->eventHandle = CreateEvent(NULL, FALSE, FALSE, "ATK 0100 Sync Event");
}

void AcpiListenerThread::run() {
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

    auto a = GetLastError();

    qDebug() << "1st init " + QString::number(result) + ", error " + QString::number(a);

    unsigned char data[8] = {};
    data[0] = ((unsigned char *)&this->eventHandle)[0];
    data[1] = ((unsigned char *)&this->eventHandle)[1];
    memset(&data[3], 0, 5);


    result = DeviceIoControl(acpiHandle,
                                     0x222400,
                                     &data[0],
                                     8,
                                     &outBuffer[0],
                                     8,
                                     &bytesReturned,
                                     NULL);

    a = GetLastError();

    qDebug() << "1st control " + QString::number(result) + ", error " + QString::number(a);

    result = WaitForSingleObject(eventHandle, INFINITE);

    qDebug() << "1st wait " + QString::number(result);

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

        qDebug() << "2nd control " + QString::number(result);

        result = WaitForSingleObject(eventHandle, INFINITE);

        qDebug() << "2nd wait " + QString::number(result);

        emittable = ((unsigned long *)&outBuffer)[0];
        emit resultReady(emittable);
    }


}


