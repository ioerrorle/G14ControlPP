#include "ACPIListenerThread.h"

ACPIListenerThread::ACPIListenerThread(HANDLE acpiHandle, QString &error) {
    this->acpiHandle = CreateFile("\\\\.\\ATKACPI\\ASLDSRV",
                                                      GENERIC_READ | GENERIC_WRITE,
                                                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                      NULL,
                                                      OPEN_EXISTING,
                                                      0,
                                                      NULL);

    this->eventHandle = CreateEvent(NULL, FALSE, FALSE, "ATK 0100 Sync Event");
    //starting service
//    HANDLE serviceFile = CreateFile("atkwmiacpi64.sys", NULL, NULL, NULL, OPEN_EXISTING, 0x80, NULL);
//    //check for INVALID_HANDLE_VALUE
//    CloseHandle(serviceFile);
//    SC_HANDLE managerHandle = OpenSCManager(NULL, NULL, 0x0f003f);
//    SC_HANDLE serviceHandle = CreateService(managerHandle,
//            "ATKWMIACPIIO_64",
//            "ATKWMIACPI Driver",
//            0x0f01ff,
//            );


}

void ACPIListenerThread::run() {
    unsigned char outBuffer[8];
    DWORD bytesReturned;

    unsigned char data[8] = {};
    data[0] = ((unsigned char *)&this->eventHandle)[0];
    data[1] = ((unsigned char *)&this->eventHandle)[1];
    memset(&data[3], 0, 5);


    WINBOOL result = DeviceIoControl(acpiHandle,
                                     0x222400,
                                     &data,
                                     8,
                                     &outBuffer[0],
                                     8,
                                     &bytesReturned,
                                     NULL);

    auto a = GetLastError();

    qDebug() << "1st control " + QString::number(result) + ", error " + QString::number(a);

    result = WaitForSingleObject(eventHandle, INFINITE);

    qDebug() << "1st wait " + QString::number(result);

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
    }


}


