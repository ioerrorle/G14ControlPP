#include <QApplication>
#include <QDebug>
#include <windows.h>
#include <src/ui/MainWindow.h>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/atkacpi/AcpiControlSingleton.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow mainWindow;

    HWND mainWindowHandle = (HWND)mainWindow.winId();

    GUID guids[7] = {GUID_CONSOLE_DISPLAY_STATE,
                     GUID_DEVICE_POWER_POLICY_VIDEO_BRIGHTNESS,
                     GUID_LIDSWITCH_STATE_CHANGE,
                     GUID_LIDCLOSE_ACTION,
                     GUID_LIDOPEN_POWERSTATE,
                     GUID_ACDC_POWER_SOURCE,
                     GUID_BATTERY_PERCENTAGE_REMAINING};

    for (int i = 0; i < 7; i++) {
        HANDLE handle = RegisterPowerSettingNotification(mainWindowHandle, &guids[i], DEVICE_NOTIFY_WINDOW_HANDLE);
        qDebug() << handle;
    }

    QString error = nullptr;
    if (!RyzenAdjSingleton::getInstance().init(error)) {
        //todo make a window for that
        return 1;
    }

    if (!AcpiControlSingleton::getInstance().init(error)) {
        //todo make a window for that
        return 1;
    }

    mainWindow.show();
    return a.exec();
}
