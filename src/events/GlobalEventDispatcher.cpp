#include <src/ui/MainWindow.h>
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

    if (!AcpiController::getInstance().init(error))
        return false;

    this->acpiListenerThread = new AcpiListenerThread(error);
    connect(this->acpiListenerThread, &AcpiListenerThread::acpiEvent, this, &GlobalEventDispatcher::handleAcpiEvent);
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
        HwProfile defaultPp = {0};
        defaultPp.armouryCratePlanId = 0;
        currentPowerPlanSet = {0};
        currentPowerPlanSet.usbPowerPlan = defaultPp;
        currentPowerPlanSet.acPowerPlan = defaultPp;
        currentPowerPlanSet.dcPowerPlan = defaultPp;

    }

    PowerSourceType currentPS = AcpiController::getInstance().getPowerSourceType();

    HwProfile powerPlan;
    switch (currentPS) {
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

    AcpiControlSingleton::getInstance().setPowerPlan(
            ARMOURY_CRATE_PLANS[powerPlan.armouryCratePlanId].asusPlanCode);

    //wait 1 sec before applying additional things because ac power plan changes not right away
    QTimer::singleShot(1000, this, [powerPlan]() {
        if (!powerPlan.fansProfile.name.isEmpty()) {
            AcpiController::getInstance().setFanProfile(powerPlan.fansProfile);
        }
        if (!powerPlan.powerProfile.name.isEmpty()) {
            CpuController.setPowerProfile(powerPlan.powerProfile); //fucking dangerous.
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
    auto currentPowerPlanSet = SETT.getPowerPlanSet(currentPowerPlanSetName);
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


