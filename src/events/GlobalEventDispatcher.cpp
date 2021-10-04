#include "GlobalEventDispatcher.h"

GlobalEventDispatcher &GlobalEventDispatcher::getInstance() {
    static GlobalEventDispatcher instance;
    return instance;
}

GlobalEventDispatcher::GlobalEventDispatcher() {
}

const GUID GlobalEventDispatcher::guids[] = {GUID_CONSOLE_DISPLAY_STATE,
                                             GUID_DEVICE_POWER_POLICY_VIDEO_BRIGHTNESS,
                                             GUID_LIDSWITCH_STATE_CHANGE,
                                             GUID_ACDC_POWER_SOURCE,
                                             GUID_BATTERY_PERCENTAGE_REMAINING};

bool GlobalEventDispatcher::nativeEventFilter(const QByteArray &eventType, void *message, long *extra) {
    MSG *msg = static_cast< MSG * >( message );

    if (msg->message == WM_POWERBROADCAST && msg->wParam == PBT_POWERSETTINGCHANGE) {
        handlePowerCfgChange(reinterpret_cast<POWERBROADCAST_SETTING *>(msg->lParam));
        //todo handle all this shit
        /*qDebug() << ("Power broadcast");
        switch (msg->wParam) {
            case PBT_APMPOWERSTATUSCHANGE:
                qDebug() << ("PBT_APMPOWERSTATUSCHANGE  received\n");
                break;
            case PBT_APMRESUMEAUTOMATIC:
                qDebug() << ("PBT_APMRESUMEAUTOMATIC  received\n");
                break;
            case PBT_APMRESUMESUSPEND:
                qDebug() << ("PBT_APMRESUMESUSPEND  received\n");
                break;
            case PBT_APMSUSPEND:
                qDebug() << ("PBT_APMSUSPEND  received\n");
                break;
        }*/

    }
    return false;
}

bool GlobalEventDispatcher::init(HWND mainWindowHandle, QString &error) {
    QString err;

    this->pressedKey = new INPUT;

    for (const auto &guid : guids) {
        HANDLE handle = RegisterPowerSettingNotification(mainWindowHandle, &guid, DEVICE_NOTIFY_WINDOW_HANDLE);

        //todo we need to unsub when app is closed
    }

    if (!AcpiControlSingleton::getInstance().init(error))
        return false;
    if (!KbdControlSingleton::getInstance().init(error))
        return false;

    //subscribe to global events
    QAbstractEventDispatcher::instance()->installNativeEventFilter(this);

    //subscribe to singletons
    connect(&AcpiControlSingleton::getInstance(), &AcpiControlSingleton::acpiEvent, this,
            &GlobalEventDispatcher::handleAcpiEvent);
    connect(&KbdControlSingleton::getInstance(), &KbdControlSingleton::kbdFnEvent, this,
            &GlobalEventDispatcher::handleKbdFnPress);

    return true;
}

void GlobalEventDispatcher::handleKbdFnPress(const unsigned char fnKeyCode) {
    QString error;

    switch (fnKeyCode) {
        case 0x00:
            this->releaseKey();
            break;
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
            KbdControlSingleton::getInstance().changeKbdBrightness(false);
            break;
        case 0x8a://v
            KbdControlSingleton::getInstance().changeKbdBrightness(true);
            break;
        case 0x10://f7
            AcpiControlSingleton::getInstance().lcdLightChange(true);
            break;
        case 0x20://f8
            AcpiControlSingleton::getInstance().lcdLightChange(false);
            break;
        case 0x7c://mute
        {
            WINBOOL mute = FALSE;
            AudioUtils::toggleMute(mute, error);
            break;
        }
        default:
            break;
    }

    qDebug() << error;
}

void GlobalEventDispatcher::sendScanCode(WORD hwScanCode, WORD vScanCode) {

    if (this->pressedKey != nullptr //dunno why i do it kekw
        && this->pressedKey->ki.wScan != hwScanCode
        && this->pressedKey->ki.wVk != vScanCode) {
        releaseKey();
    }

    *this->pressedKey = {};

    // Set up a generic keyboard event.
    this->pressedKey->type = INPUT_KEYBOARD;
    this->pressedKey->ki.wScan = hwScanCode; // hardware scan code for key
    this->pressedKey->ki.time = 0;
    this->pressedKey->ki.dwExtraInfo = 0;

    // Press the "A" key
    this->pressedKey->ki.wVk = vScanCode; // virtual-key code for the "a" key
    this->pressedKey->ki.dwFlags = 0; // 0 for key press

    SendInput(1, this->pressedKey, sizeof(INPUT));
}

void GlobalEventDispatcher::releaseKey() {
    if (this->pressedKey == nullptr) {
        return;
    }

    this->pressedKey->ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(1, this->pressedKey, sizeof(INPUT));
}

void GlobalEventDispatcher::handleAcpiEvent(const unsigned long acpiCode) {

}

void GlobalEventDispatcher::handlePowerCfgChange(POWERBROADCAST_SETTING *settings) {
    if (settings->PowerSetting == GUID_LIDSWITCH_STATE_CHANGE) {
        KbdControlSingleton::getInstance().toggleKbdBacklight(settings->Data[0]);
    }
}


