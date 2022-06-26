#include "keyboardcontroller.h"

KeyboardController::KeyboardController(HotkeysProfile *profile, QObject *parent)
    : QObject{parent}
    , profile(profile)
{

}

bool KeyboardController::init(QString &error)
{
//    QLibrary hidLib("hid.dll");
//    QLibrary lib("cfgmgr32.dll");

//    typedef unsigned long (*CM_Get_Device_Interface_List_Size_Fn)(unsigned long *pulLen, LPGUID InterfaceClassGuid,
//                                                                  char *pDeviceID, unsigned long ulFlags);
//    CM_Get_Device_Interface_List_Size_Fn CM_Get_Device_Interface_List_Size_Ptr = NULL;

//    typedef unsigned long (*CM_Get_Device_Interface_List_Fn)(LPGUID InterfaceClassGuid, char *pDeviceID,
//                                                             char *Buffer, unsigned long BufferLen,
//                                                             unsigned long ulFlags);
//    CM_Get_Device_Interface_List_Fn CM_Get_Device_Interface_List_Ptr = NULL;

//    if (lib.load() && hidLib.load())
//    {
//        CM_Get_Device_Interface_List_Size_Ptr =
//                (CM_Get_Device_Interface_List_Size_Fn)  lib.resolve(
//                                                        "CM_Get_Device_Interface_List_SizeA");


//        CM_Get_Device_Interface_List_Ptr =
//                (CM_Get_Device_Interface_List_Fn)   lib.resolve(
//                                                    "CM_Get_Device_Interface_ListA");

//        HidD_SetFeature_Ptr = (HidD_SetFeature_Fn) hidLib.resolve("HidD_SetFeature");
//    }
//    else
//    {
//        error = QString::asprintf("Cannot load cfgmgr32.dll/hid.dll library");
//        return false;
//    }

//    if (
//        !CM_Get_Device_Interface_List_Size_Ptr
//        || !CM_Get_Device_Interface_List_Ptr
//        || !HidD_SetFeature_Ptr)
//    {
//        error = QString::asprintf("Cannot find cfgmgr32.dll/hid.dll functions");
//        return false;
//    }

    unsigned long DeviceInterfaceListLength = 0;

    unsigned long cr = CM_Get_Device_Interface_List_SizeA(
                &DeviceInterfaceListLength,
                (LPGUID) &GUID_DEVINTERFACE_HID,
                NULL,
                0);

    if (cr != 0)
    {
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

    cr = CM_Get_Device_Interface_ListA((LPGUID) &GUID_DEVINTERFACE_HID,
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

    for (int deviceIndex = 0; deviceIndex < devices.size(); deviceIndex++) {
        if (devices.at(deviceIndex).contains("VID_0B05")
                && devices.at(deviceIndex).contains("PID_1866")
                && devices.at(deviceIndex).contains("Col01"))
        {
            kbdHandle = createKbdHandle(devices.at(deviceIndex));
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

    connect(kbdListenerThread, &KbdListenerThread::fnKeyPressCaptured, this, &KeyboardController::onFnKeyPressed);
    connect(kbdListenerThread, &KbdListenerThread::finished, kbdListenerThread, &KbdListenerThread::deleteLater);
    kbdListenerThread->start();
    return true;
}

void KeyboardController::onFnKeyPressed(const unsigned char fnKeyCode)
{
    switch (fnKeyCode) {
    case 0x00:
        releaseKey();
        break;
    case 0xc4://up
        processFnKeyAction(profile->kbdBrU);
        break;
    case 0xc5://down
        processFnKeyAction(profile->kbdBrD);
        break;
    case 0xb2://left
        processFnKeyAction(profile->leftButton);
        break;
    case 0xb3://right
        processFnKeyAction(profile->rightButton);
        break;
    case 0x9e: //C
        processFnKeyAction(profile->cButton);
        break;
    case 0x8a: //V
        processFnKeyAction(profile->vButton);
        break;
    case 0x10://f7
        processFnKeyAction(profile->lcdBrD);
        break;
    case 0x20://f8
        processFnKeyAction(profile->lcdBrU);
        break;
    case 0x7c: //mute
        processFnKeyAction(profile->mute);
        break;
    case 0x6c: //sleep
        processFnKeyAction(profile->sleep);
        break;
    case 0x88: //rfkill
        processFnKeyAction(profile->plane);
        break;
    case 0x38: //rog button
        processFnKeyAction(profile->rog);
        break;
    case 0xae: //fan button
        processFnKeyAction(profile->fan);
        break;
    case 0x6b: //toggle touchpad
        processFnKeyAction(profile->touchpad);
        break;
    default:
        break;
    }
}

void KeyboardController::setKbdBrightness(const unsigned char kbdBr)
{
    unsigned char data[0x40] = {0x5a, 0xba, 0xc5, 0xc4, kbdBr};
    memset(&data[5], 0, sizeof(data) - 5);
    sendHidControl(data, 0x40);
}

HANDLE KeyboardController::createKbdHandle(const QString &devicePath) {
    HANDLE result = CreateFileA(
                devicePath.toLocal8Bit().constData(),
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                NULL
                );

    return result;
}

void KeyboardController::processFnKeyAction(const HotkeyAction &hotkeyAction) {
    /*QString error;
    uchar actionCode = action.action;
    switch (actionCode) {
    case HOTKEY_ACTION_MUTE:
        AudioUtils::toggleMute(result, error);
        break;
    case HOTKEY_ACTION_CALL_G14CPP:
        for (QWidget *topLevelWidget : QApplication::topLevelWidgets()) {
            auto *mainWindow = qobject_cast<MainWindow*>(topLevelWidget);
            if (mainWindow != nullptr) {
                mainWindow->bringToFront();
                break;
            }
        }
        break;
    case HOTKEY_ACTION_CHANGE_POWER_PLAN:
        switchToNextPowerPlanSet();
        break;
    case HOTKEY_ACTION_LCD_BR_DOWN:
        AcpiControlSingleton::getInstance().lcdLightChange(true);
        break;
    case HOTKEY_ACTION_LCD_BR_UP:
        AcpiControlSingleton::getInstance().lcdLightChange(false);
        break;
    case HOTKEY_ACTION_TOGGLE_TOUCHPAD:
        KbdControlSingleton::getInstance().toggleTouchPad();
        break;
    case HOTKEY_ACTION_PLANE_MODE:
        AcpiControlSingleton::getInstance().sendRfKillCommand();
        break;
    case HOTKEY_ACTION_SLEEP:
        AcpiControlSingleton::getInstance().sendSleepCommand();
        break;
    case HOTKEY_ACTION_KBD_BR_DOWN: {
        uchar kbdBr = KbdControlSingleton::getInstance().changeKbdBrightness(false);
        SETT.putKbdBr(kbdBr);
        break;
    }
    case HOTKEY_ACTION_KBD_BR_UP: {
        uchar kbdBr = KbdControlSingleton::getInstance().changeKbdBrightness(true);
        SETT.putKbdBr(kbdBr);
        break;
    }
    case HOTKEY_ACTION_BUTTON: {
        sendScanCode(action.data, action.modifiers);
        break;
    }

    }*/
}

void KeyboardController::sendScanCode(quint32 hwScanCode, quint32 modifiers) {
    Q_UNUSED(modifiers)
    if (this->pressedKey != nullptr
            && this->pressedKey->ki.wScan != hwScanCode) {
        releaseKey();
    }

    *this->pressedKey = {};

    this->pressedKey->type = INPUT_KEYBOARD;
    this->pressedKey->ki.wScan = hwScanCode;
    this->pressedKey->ki.time = 0;
    this->pressedKey->ki.dwExtraInfo = 0;

    this->pressedKey->ki.dwFlags = KEYEVENTF_SCANCODE | (hwScanCode >> 8);

    SendInput(1, this->pressedKey, sizeof(INPUT));
}

void KeyboardController::releaseKey() {
    if (this->pressedKey == nullptr) {
        return;
    }

    this->pressedKey->ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE | (this->pressedKey->ki.wScan >> 8);
    SendInput(1, this->pressedKey, sizeof(INPUT));
}

void KeyboardController::sendHidControl(unsigned char *data, unsigned short size) {
    WINBOOL result = HidD_SetFeature(kbdHandle, data, size);
    if (!result) {
        DWORD lastError = GetLastError();
    }
}
