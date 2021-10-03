#include "KbdListenerThread.h"

KbdListenerThread::KbdListenerThread(HANDLE *device, QString &error) {
    error = nullptr;
    this->kbdHandle = device;
    kbdEventHandle = CreateEvent(NULL, FALSE, FALSE, "KbdEvent");

    kbdReadResult = new char[6];
    kbdReadCount = new DWORD(0);
    kbdOverlap = new OVERLAPPED();
    *kbdOverlap = {0};
    kbdOverlap->hEvent = this->kbdEventHandle;
}

void KbdListenerThread::run() {
    {
        QString error;
        int lastError = 0;
        while (!isFinished()) {
            WINBOOL readResult = ReadFile(*this->kbdHandle,
                                          kbdReadResult,
                                          6,
                                          kbdReadCount,
                                          kbdOverlap);
            lastError = GetLastError();
            if (!readResult && lastError != ERROR_IO_PENDING) {
                error = tr("Device doesn't support defined reading");
                break;
            }

            DWORD waitResult = WaitForSingleObject(kbdEventHandle, INFINITE);
            lastError = GetLastError();
            if (readResult == WAIT_FAILED && lastError != ERROR_IO_PENDING) {
                error = tr("Waiting for kb press failed");
                break;
            }

            DWORD readCount = *kbdReadCount;
            unsigned char *readBuffer = reinterpret_cast<unsigned char *>(kbdReadResult);

            emitKeyPress(readBuffer, 6);

            *kbdOverlap = {};
            kbdOverlap->hEvent = kbdEventHandle;
        }
    }
}


void KbdListenerThread::emitKeyPress(const unsigned char *cc, int nCount) {

    if (nCount < 2) {
        return;
        //looks like it's not what we are looking for
    }
    if (cc[1] == 0xec) {
        //it's Fn press or something like that, returning.
        return;
    }

    emit resultReady(cc[1]);
}

/*

void KbdListenerThread::sendHidControl(unsigned char *data) {
    WINBOOL result = HidD_SetFeature_Ptr(kbdHandles->at(0), data, 0x40);
    if (!result) {
        DWORD lastError = GetLastError();
        qDebug() << lastError;
    }
}

void KbdListenerThread::changeKbdBrightness(bool increase) {
    if (increase && this->kbdBr >= 3) {
        this->kbdBr = 3;
    } else if (!increase && this->kbdBr <= 0) {
        this->kbdBr = 0;
    } else {
        if (increase) {
            this->kbdBr++;
        } else {
            this->kbdBr--;
        }
    }
    unsigned char data[0x40] = {0x5a, 0xba, 0xc5, 0xc4, this->kbdBr};
    memset(&data[5], 0, sizeof(data) - 5);
    this->sendHidControl(data);
}*/
