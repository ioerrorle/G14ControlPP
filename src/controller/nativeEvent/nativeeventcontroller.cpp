#include "nativeeventcontroller.h"
#include <QAbstractEventDispatcher>

const GUID NativeEventController::EventGUIDs[] = {GUID_CONSOLE_DISPLAY_STATE,
                                             GUID_DEVICE_POWER_POLICY_VIDEO_BRIGHTNESS,
                                             GUID_LIDSWITCH_STATE_CHANGE,
                                             GUID_ACDC_POWER_SOURCE,
                                             GUID_BATTERY_PERCENTAGE_REMAINING};

NativeEventController::NativeEventController(QObject *parent)
    : QObject{parent}
{

}

bool NativeEventController::init(HWND mainWindowHandle, QString &error) {
    for (const GUID &guid : EventGUIDs) {
        HANDLE handle = RegisterPowerSettingNotification(mainWindowHandle, &guid, DEVICE_NOTIFY_WINDOW_HANDLE);

        //todo we need to unsub when app is closed
    }

    //subscribe to global events
    QAbstractEventDispatcher::instance()->installNativeEventFilter(this);

    return true;
}

bool NativeEventController::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *) {
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

void NativeEventController::handlePowerCfgChange(POWERBROADCAST_SETTING *settings)
{
    if (settings->PowerSetting == GUID_LIDSWITCH_STATE_CHANGE) {
        UCHAR lidState = settings->Data[0];
        emit lidStateChanged(lidState);
    }
    if (settings->PowerSetting == GUID_ACDC_POWER_SOURCE) {
        UCHAR powerSource = settings->Data[0];
        emit powerSourceChanged(powerSource);
    }
}

void NativeEventController::handleWakeUp()
{
    emit wakeUpReceived();
}
