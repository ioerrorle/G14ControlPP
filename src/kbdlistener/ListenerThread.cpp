#include "ListenerThread.h"

ListenerThread::ListenerThread(QString &error) {
    QLibrary hidLib("hid.dll");
    QLibrary lib("cfgmgr32.dll");
    if (hidLib.load() && lib.load()) {

        typedef unsigned long (*CM_Get_Device_Interface_List_Size_Fn)(unsigned long *pulLen, LPGUID InterfaceClassGuid,
                                                                      char *pDeviceID, unsigned long ulFlags);
        CM_Get_Device_Interface_List_Size_Fn CM_Get_Device_Interface_List_Size_Ptr = NULL;
        CM_Get_Device_Interface_List_Size_Ptr = (CM_Get_Device_Interface_List_Size_Fn) lib.resolve(
                "CM_Get_Device_Interface_List_SizeA");

        typedef unsigned long (*CM_Get_Device_Interface_List_Fn)(LPGUID InterfaceClassGuid, char *pDeviceID,
                                                                 char *Buffer, unsigned long BufferLen,
                                                                 unsigned long ulFlags);
        CM_Get_Device_Interface_List_Fn CM_Get_Device_Interface_List_Ptr = NULL;
        CM_Get_Device_Interface_List_Ptr = (CM_Get_Device_Interface_List_Fn) lib.resolve(
                "CM_Get_Device_Interface_ListA");


        HidD_SetFeature_Ptr = (HidD_SetFeature_Fn) hidLib.resolve(
                "HidD_SetFeature");

        if (CM_Get_Device_Interface_List_Size_Ptr
            && CM_Get_Device_Interface_List_Ptr
            && HidD_SetFeature_Ptr) {
            unsigned long DeviceInterfaceListLength = 0;

            unsigned long cr = CM_Get_Device_Interface_List_Size_Ptr(&DeviceInterfaceListLength,
                                                                     (LPGUID) &GUID_DEVINTERFACE_HID, NULL, 0);

            if (cr != 0) {
                //TODO ERROR
                return;
            }

            //qDebug() << DeviceInterfaceListLength;

            PSTR DeviceInterfaceList = (PSTR) HeapAlloc(GetProcessHeap(),
                                                        HEAP_ZERO_MEMORY,
                                                        DeviceInterfaceListLength * sizeof(CHAR));

            if (DeviceInterfaceList == NULL) {
                cr = CR_OUT_OF_MEMORY;
                //TODO ERROR
                return;
            }

            cr = CM_Get_Device_Interface_List_Ptr((LPGUID) &GUID_DEVINTERFACE_HID,
                                                  NULL,
                                                  (PCHAR) DeviceInterfaceList,
                                                  DeviceInterfaceListLength,
                                                  CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

            if (cr != 0) {
                //TODO ERROR
                return;
            }

            QStringList devices = QString::fromLocal8Bit(DeviceInterfaceList, DeviceInterfaceListLength - 1).split(
                    QChar(0x00));

            kbdEventHandles = new QVector<HANDLE>;
            kbdHandles = new QVector<HANDLE>;
            kbdOverlaps = new QVector<LPOVERLAPPED>;
            kbdReadResults = new QVector<LPVOID>;
            kbdReadCount = new QVector<LPDWORD>;
            pressedKeys = new QVector<INPUT>();

            int index = 0;
            for (int i = 0; i < devices.size(); i++) {
                if (devices[i].contains(
                        "VID_0B05") && devices[i].contains("PID_1866") && devices[i].contains("Col01")) {
                    this->createReadFileJob(devices[i]);
                    index++;
                }
            }
        }
    } else {
        //TODO ERROR
        return;
    }

    //TODO OK
    return;
}

void ListenerThread::run() {
    {
        while (!isFinished()) {
            DWORD result = WaitForMultipleObjects(kbdEventHandles->size(), kbdEventHandles->constData(), false,
                                                  INFINITE);
            int lastError = GetLastError();
            if (result == WAIT_FAILED && lastError != ERROR_IO_PENDING) {
                qDebug() << "Wait for multiple objs object";
                qDebug() << lastError;
            } else {
                unsigned long objIndex = result - WAIT_OBJECT_0;

                if (objIndex < 0 || objIndex > kbdEventHandles->size()) {
                    qDebug() << "Object index error";
                    qDebug() << objIndex;
                } else {

                    LPOVERLAPPED oldOverlapped = kbdOverlaps->at(objIndex);

                    DWORD bytesRead = *kbdReadCount->at(objIndex);
                    unsigned char *bytes = reinterpret_cast<unsigned char *>( kbdReadResults->at(objIndex));

                    //if (bytes[1] != 0xec) {
                    sendInputIfNeeded(bytes, 6);
                    //}

                    *oldOverlapped = {};
                    (*oldOverlapped).hEvent = kbdEventHandles->at(objIndex);

                    WINBOOL result1 = ReadFile(kbdHandles->at(objIndex),
                                               kbdReadResults->at(objIndex),
                                               6,
                                               kbdReadCount->at(objIndex),
                                               kbdOverlaps->at(objIndex));
                    lastError = GetLastError();
                    if (!result1 && lastError != ERROR_IO_PENDING) {
                        qDebug() << "Second read";
                        qDebug() << lastError;
                    }
                }
            }
        }
    }
}

void ListenerThread::createReadFileJob(QString &qString) {
    int index = kbdEventHandles->size();

    HANDLE eventHandle = CreateEvent(NULL, FALSE, FALSE, QString::number(index).toStdString().c_str());

    HANDLE fileHandle = CreateFile(
            qString.toLocal8Bit().constData(),
            GENERIC_READ/* | GENERIC_WRITE*/,
            FILE_SHARE_READ/* | FILE_SHARE_WRITE*/,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            NULL
    );

    LPVOID readResults = new char[6];

    DWORD readCount = 0xFF;

    OVERLAPPED overlapped = {0};
    overlapped.hEvent = eventHandle;

    WINBOOL result1 = ReadFile(fileHandle,
                               readResults,
                               6,
                               &readCount,
                               &overlapped);
    int a = GetLastError();
    if (!result1 && a != ERROR_IO_PENDING) {
        qDebug() << "First read";
        qDebug() << a;
    } else {
        this->kbdEventHandles->append(eventHandle);
        this->kbdHandles->append(fileHandle);
        this->kbdReadResults->append(readResults);
        this->kbdReadCount->append(&readCount);
        this->kbdOverlaps->append(&overlapped);
    }
}

void ListenerThread::sendInputIfNeeded(const unsigned char *cc, int nCount) {
    if (nCount < 2) {
        return;
        //looks like it's not what we are looking for
    }
    if (cc[1] == 0xec) {
        //it's Fn press or something like that, returning.
        return;
    }
    if (cc[1] == 0x00) {
        //all keys are released
        this->releaseKey();
        return;
    }

    for (int i = 1; i < nCount; i++) {
        unsigned char c = cc[i];
        switch (c) {
            case 0xc4://up
                this->sendScanCode(0xe049, VK_PRIOR);
                break;

            case 0xc5://down
                this->sendScanCode(0xe051, VK_NEXT);
                break;

            case 0xb2://left
                this->sendScanCode(0xe047, VK_HOME);
                break;
            case 0xb3://right
                this->sendScanCode(0xe04f, VK_END);
                break;
            case 0x9e://C
                this->changeKbdBrightness(false);
                break;
            case 0x8a://v
                this->changeKbdBrightness(true);
                break;
            case 0x10://f7
                AcpiControlSingleton::getInstance().lcdLightChange(true);
                break;
            case 0x20://f8
                AcpiControlSingleton::getInstance().lcdLightChange(false);
                break;
            default:
                break;
        }
    }
}

void ListenerThread::sendScanCode(WORD hwScanCode, WORD vScanCode) {

    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = hwScanCode; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "A" key
    ip.ki.wVk = vScanCode; // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press

    this->pressedKeys->append(ip);

    SendInput(1, &ip, sizeof(INPUT));
}

void ListenerThread::releaseKey() {
    if (this->pressedKeys == NULL) {
        return;
    }

    UINT size = this->pressedKeys->length();

    INPUT ips[size];

    for (int i = 0; i < size; i++) {
        INPUT ip = this->pressedKeys->at(i);
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        ips[i] = ip;
    }

    SendInput(size, &ips[0], sizeof(INPUT));
    this->pressedKeys->clear();
}

void ListenerThread::sendHidControl(unsigned char *data) {
    WINBOOL result = HidD_SetFeature_Ptr(kbdHandles->at(0), data, 0x40);
    if (!result) {
        DWORD lastError = GetLastError();
        qDebug() << lastError;
    }
}

void ListenerThread::changeKbdBrightness(bool increase) {
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
}
