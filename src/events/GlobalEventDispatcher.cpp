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

    if (msg->message == WM_POWERBROADCAST) {
        switch (msg->wParam) {
            case PBT_POWERSETTINGCHANGE:
                handlePowerCfgChange(reinterpret_cast<POWERBROADCAST_SETTING *>(msg->lParam));
                break;
            case PBT_APMRESUMEAUTOMATIC:
                handleWakeUp();
                break;
        }
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
    //read settings


    for (const auto &guid : guids) {
        HANDLE handle = RegisterPowerSettingNotification(mainWindowHandle, &guid, DEVICE_NOTIFY_WINDOW_HANDLE);

        //todo we need to unsub when app is closed
    }

    if (!AcpiControlSingleton::getInstance().init(error))
        return false;

    this->acpiListenerThread = new AcpiListenerThread(error);
    connect(this->acpiListenerThread, &AcpiListenerThread::resultReady, this, &GlobalEventDispatcher::handleAcpiEvent);
    connect(this->acpiListenerThread, &AcpiListenerThread::finished, this->acpiListenerThread,
            &QObject::deleteLater);
    this->acpiListenerThread->start();
    if (!KbdControlSingleton::getInstance().init(error))
        return false;

    readSettings();

    //subscribe to global events
    QAbstractEventDispatcher::instance()->installNativeEventFilter(this);


    connect(&KbdControlSingleton::getInstance(), &KbdControlSingleton::kbdFnEvent, this,
            &GlobalEventDispatcher::handleKbdFnPress);

    return true;
}

void GlobalEventDispatcher::readSettings() {
    uchar kbdBr = SETT.getKbdBr();
    KbdControlSingleton::getInstance().setKbdBrightness(kbdBr);
}

void GlobalEventDispatcher::handleKbdFnPress(const unsigned char fnKeyCode) {
    QString error;

    switch (fnKeyCode) {
        case 0x00:
            this->releaseKey();
            break;
        case 0xc4://up
            this->sendScanCode(VK_PRIOR);
            break;
        case 0xc5://down
            this->sendScanCode(VK_NEXT);
            break;
        case 0xb2://left
            this->sendScanCode(VK_HOME);
            break;
        case 0xb3://right
            this->sendScanCode(VK_END);
            break;
        case 0x9e: {//C
            uchar kbdBr = KbdControlSingleton::getInstance().changeKbdBrightness(false);
            SETT.putKbdBr(kbdBr);
            break;
        }
        case 0x8a: {//v
            uchar kbdBr = KbdControlSingleton::getInstance().changeKbdBrightness(true);
            SETT.putKbdBr(kbdBr);
            break;
        }
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
        case 0x6c: //sleep
            AcpiControlSingleton::getInstance().sendSleepCommand();
            break;
        case 0x88: //rfkill
            AcpiControlSingleton::getInstance().sendRfKillCommand();
            break;
        case 0x38: //rog button
            break;
        case 0xae: //fan button
            switchToNextPowerPlanSet();
            break;
        case 0x6b: //toggle touchpad
            KbdControlSingleton::getInstance().toggleTouchPad();
            break;
        default:
            break;
    }

    //qDebug() << error;
}

void GlobalEventDispatcher::sendScanCode(WORD vScanCode) {

    if (this->pressedKey != nullptr //dunno why i do it kekw
        //&& this->pressedKey->ki.wScan != hwScanCode
        && this->pressedKey->ki.wVk != vScanCode) {
        releaseKey();
    }

    *this->pressedKey = {};

    // Set up a generic keyboard event.
    this->pressedKey->type = INPUT_KEYBOARD;
    //this->pressedKey->ki.wScan = hwScanCode; // hardware scan code for key
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
    qDebug() << acpiCode;
}

void GlobalEventDispatcher::handlePowerCfgChange(POWERBROADCAST_SETTING *settings) {
    //qDebug() << "Power event!";
    if (settings->PowerSetting == GUID_LIDSWITCH_STATE_CHANGE) {
        KbdControlSingleton::getInstance().toggleKbdBacklight(settings->Data[0]);
    }
    if (settings->PowerSetting == GUID_ACDC_POWER_SOURCE) {
        applyPowerPlanFromCurrentSet();
        //check power source
        //qDebug() << AcpiControlSingleton::getInstance().getPowerSourceType();
        //settings->Data[0]; // o for AC, 1 for battery
    }
}

void GlobalEventDispatcher::applyPowerPlanFromCurrentSet() {
    auto currentPowerPlanSet = SETT.getCurrentPowerPlanSet();
    if (currentPowerPlanSet.name.isEmpty()) {
        return;
    }

    PowerSourceType currentPS = AcpiControlSingleton::getInstance().getPowerSourceType();

    PowerPlan powerPlan;
    switch(currentPS) {
        case POWER_SOURCE_BATTERY:
            powerPlan = currentPowerPlanSet.dcPowerPlan;
            break;
        case POWER_SOURCE_180W:
            powerPlan = currentPowerPlanSet.acPowerPlan;
            break;
        case POWER_SOURCE_USB:
            powerPlan = currentPowerPlanSet.usbPowerPlan;
            break;
        default:
            powerPlan = {0x00};
    }

    AcpiControlSingleton::getInstance().setPowerPlan(ARMOURY_CRATE_PLANS[powerPlan.armouryCratePlanId].asusPlanCode);

    //wait 1 sec before applying additional things because ac power plan changes not right away
    QTimer::singleShot(1000, this, [powerPlan] () {
        if (!powerPlan.fansProfile.name.isEmpty()) {
            AcpiControlSingleton::getInstance().setFanProfile(powerPlan.fansProfile);
        }
        if (!powerPlan.powerProfile.name.isEmpty()) {
            RY.setPowerProfile(powerPlan.powerProfile); //fucking dangerous.
        }
    });
}

void GlobalEventDispatcher::switchToNextPowerPlanSet() {
    auto usedPowerPlans = SETT.getUsedPowerPlans();
    if (usedPowerPlans.isEmpty()) {
        //we have no saved for fast switching power plans
        return;
    }
    QString currentPowerPlanSetName = SETT.getCurrentPowerPlanSetName();
    if (!usedPowerPlans.contains(currentPowerPlanSetName)) {
        //current power plan set is not in the list, we will use first from the list
        currentPowerPlanSetName = usedPowerPlans[0];
    } else {
        int index = (usedPowerPlans.indexOf(currentPowerPlanSetName) + 1) % usedPowerPlans.size();
        currentPowerPlanSetName = usedPowerPlans[index];
    }
    //get this power plan from the settings
    auto currentPowerPlanSet = SETT.getPowerPlanSetByName(currentPowerPlanSetName);
    if (currentPowerPlanSet.name.isEmpty()) {
        //for some reason we've got invalid set
        return;
    }
    //save power plan set as current
    SETT.setCurrentPowerPlanSetName(currentPowerPlanSetName);
    applyPowerPlanFromCurrentSet();
    //create notification
    auto title = tr("Power plan changed");
    auto message = tr("Current power plan is ") + currentPowerPlanSetName;
    NotificationHelper::getInstance().showMessage(title, message);
}

void GlobalEventDispatcher::handleWakeUp() {
    applyPowerPlanFromCurrentSet();
    KbdControlSingleton::getInstance().toggleKbdBacklight(true);
}


