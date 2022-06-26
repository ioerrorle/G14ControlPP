#include "KbdControlSingleton.h"

KbdControlSingleton &KbdControlSingleton::getInstance() {
    static KbdControlSingleton instance;
    return instance;
}

KbdControlSingleton::KbdControlSingleton() {

}

bool KbdControlSingleton::init(QString &error) {
    QLibrary hidLib("hid.dll");
    QLibrary lib("cfgmgr32.dll");

    typedef unsigned long (*CM_Get_Device_Interface_List_Size_Fn)(unsigned long *pulLen, LPGUID InterfaceClassGuid,
                                                                  char *pDeviceID, unsigned long ulFlags);
    CM_Get_Device_Interface_List_Size_Fn CM_Get_Device_Interface_List_Size_Ptr = NULL;

    typedef unsigned long (*CM_Get_Device_Interface_List_Fn)(LPGUID InterfaceClassGuid, char *pDeviceID,
                                                             char *Buffer, unsigned long BufferLen,
                                                             unsigned long ulFlags);
    CM_Get_Device_Interface_List_Fn CM_Get_Device_Interface_List_Ptr = NULL;

    if (lib.load() && hidLib.load())
    {
        CM_Get_Device_Interface_List_Size_Ptr = (CM_Get_Device_Interface_List_Size_Fn) lib.resolve(
                    "CM_Get_Device_Interface_List_SizeA");


        CM_Get_Device_Interface_List_Ptr = (CM_Get_Device_Interface_List_Fn) lib.resolve(
                    "CM_Get_Device_Interface_ListA");

        HidD_SetFeature_Ptr = (HidD_SetFeature_Fn) hidLib.resolve("HidD_SetFeature");
    }
    else
    {
        error = QString::asprintf("Cannot load cfgmgr32.dll/hid.dll library");
        return false;
    }

    if (!CM_Get_Device_Interface_List_Size_Ptr
            && !CM_Get_Device_Interface_List_Ptr
            && !HidD_SetFeature_Ptr)
    {
        error = QString::asprintf("Cannot find cfgmgr32.dll/hid.dll functions");
        return false;
    }

    unsigned long DeviceInterfaceListLength = 0;

    unsigned long cr = CM_Get_Device_Interface_List_Size_Ptr(&DeviceInterfaceListLength,
                                                             (LPGUID) &GUID_DEVINTERFACE_HID, NULL, 0);

    if (cr != 0) {
        error = QString::asprintf("Cannot exec CM_Get_Device_Interface_List_Size function, error code %lu", cr);
        return false;
    }

    PSTR DeviceInterfaceList = (PSTR) HeapAlloc(GetProcessHeap(),
                                                HEAP_ZERO_MEMORY,
                                                DeviceInterfaceListLength * sizeof(CHAR));

    if (DeviceInterfaceList == NULL) {
        //cr = CR_OUT_OF_MEMORY;
        error = QString::asprintf("Cannot allocate CM_Get_Device_Interface_List_Size");
        return false;
    }

    cr = CM_Get_Device_Interface_List_Ptr((LPGUID) &GUID_DEVINTERFACE_HID,
                                          NULL,
                                          DeviceInterfaceList,
                                          DeviceInterfaceListLength,
                                          CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

    if (cr != 0) {
        error = QString::asprintf("Cannot exec CM_Get_Device_Interface_List function, error code %lu", cr);
        return false;
    }

    QStringList devices = QString::fromLocal8Bit(DeviceInterfaceList, DeviceInterfaceListLength - 1).split(
                QChar(0x00));

    for (int i = 0; i < devices.size(); i++) {
        if (devices.at(i).contains("VID_0B05")
                && devices.at(i).contains("PID_1866")
                && devices.at(i).contains("Col01")) {
            kbdHandle = createKbdHandle(devices.at(i));
            if (kbdHandle == INVALID_HANDLE_VALUE) {
                error = QString::asprintf("Error creating kbd handle");
                return false;
            }
            break;
        }
    }
    if (kbdHandle == nullptr) {
        error = QString::asprintf("Couldn't find matching device");
        return false;
    }

    kbdListenerThread = new KbdListenerThread(&kbdHandle, error);

    connect(kbdListenerThread, &KbdListenerThread::resultReady, this, &KbdControlSingleton::handleKbdFnPress);
    connect(kbdListenerThread, &KbdListenerThread::finished, kbdListenerThread, &KbdListenerThread::deleteLater);
    kbdListenerThread->start();
    return true;
}

HANDLE KbdControlSingleton::createKbdHandle(const QString &qString) {
    HANDLE result = CreateFileA(
                qString.toLocal8Bit().constData(),
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                NULL
                );

    return result;
}

void KbdControlSingleton::sendHidControl(unsigned char *data, unsigned short size) {
    WINBOOL result = HidD_SetFeature_Ptr(kbdHandle, data, size);
    if (!result) {
        DWORD lastError = GetLastError();
    }
}

void KbdControlSingleton::setKbdBrightness(uchar &kbdBr) {
    if (kbdBr > 3) {
        kbdBr = 3;
    }

    this->kbdBr = kbdBr;

    unsigned char data[0x40] = {0x5a, 0xba, 0xc5, 0xc4, this->kbdBr};
    memset(&data[5], 0, sizeof(data) - 5);
    this->sendHidControl(data, 0x40);
}

void KbdControlSingleton::toggleTouchPad() {
    unsigned char data[0x40] = {0x5a, 0xf4, 0x6b};
    memset(&data[3], 0, sizeof(data) - 3);
    this->sendHidControl(data, 0x40);
}

uchar KbdControlSingleton::changeKbdBrightness(bool increase) {
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

    setKbdBrightness(this->kbdBr);
    return this->kbdBr;
}

void KbdControlSingleton::toggleKbdBacklight(bool enabled) {
    unsigned char tmpKbdBrightness = 0;
    if (enabled) {
        tmpKbdBrightness = this->kbdBr;
    }
    unsigned char data[0x40] = {0x5a, 0xba, 0xc5, 0xc4, tmpKbdBrightness};
    memset(&data[5], 0, sizeof(data) - 5);
    this->sendHidControl(data, 0x40);
    //todo save kb brightness
}

void KbdControlSingleton::handleKbdFnPress(const unsigned char fnKeyCode) {
    emit kbdFnEvent(fnKeyCode);
}
