#include "acpilistenerthread.h"

AcpiListenerThread::AcpiListenerThread() {
    this->eventHandle = CreateEventA(NULL, FALSE, FALSE, "ATK 0100 Sync Event");
}

void AcpiListenerThread::run() {
    unsigned char *outBuffer = new unsigned char[16];

    //very questionable code right there
    unsigned char *data = new unsigned char[8];;
    data[0] = ((unsigned char *) &this->eventHandle)[0];
    data[1] = ((unsigned char *) &this->eventHandle)[1];
    memset(&data[2], 0, 6);

    emit requestAtkAcpiCommand(0x222400, data, 8, outBuffer, 16);

    //todo check result

    //auto errorCode = GetLastError();

    //todo check result

    WINBOOL result = WaitForSingleObject(eventHandle, INFINITE);

    //todo check result

    delete [] data;

    unsigned long acpiCode = ((unsigned long *) &outBuffer)[0];
    emit acpiEvent(acpiCode);

    while (!isFinished()) {
        emit requestAtkAcpiCommand(0x222408, NULL, 0, outBuffer, 4);
        //todo check result

        result = WaitForSingleObject(eventHandle, INFINITE);

        //todo check result

        acpiCode = ((unsigned long *) &outBuffer)[0];
        emit acpiEvent(acpiCode);
    }

    delete [] outBuffer;
}
