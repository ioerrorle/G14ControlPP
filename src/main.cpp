#include <QApplication>
#include <windows.h>
#include <src/ui/MainWindow.h>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/events/GlobalEventDispatcher.h>

int main(int argc, char *argv[]) {
    QMetaType::registerComparators<FanCurve>();
    QMetaType::registerComparators<FansProfile>();
    QMetaType::registerComparators<PowerProfile>();
    QMetaType::registerComparators<PowerPlan>();
    QMetaType::registerComparators<PowerPlanSet>();
    QMetaType::registerComparators<HotkeyAction>();
    QMetaType::registerComparators<HotkeysProfile>();
    qRegisterMetaTypeStreamOperators<FanCurve>("FanCurve");
    qRegisterMetaTypeStreamOperators<FansProfile>("FansProfile");
    qRegisterMetaTypeStreamOperators<PowerProfile>("PowerProfile");
    qRegisterMetaTypeStreamOperators<PowerPlan>("PowerPlan");
    qRegisterMetaTypeStreamOperators<PowerPlanSet>("PowerPlanSet");
    qRegisterMetaTypeStreamOperators<HotkeyAction>("HotkeyAction");
    qRegisterMetaTypeStreamOperators<HotkeysProfile>("HotkeysProfile");
    qRegisterMetaTypeStreamOperators<QList<FanCurve>>("FanCurveList");
    qRegisterMetaTypeStreamOperators<QList<FansProfile>>("FansProfileList");
    qRegisterMetaTypeStreamOperators<QList<PowerProfile>>("PowerProfileList");
    qRegisterMetaTypeStreamOperators<QList<PowerPlanSet>>("PowerPlanSetList");
    //qRegisterMetaTypeStreamOperators<QMap<int, PowerPlan>>("PowerPlanMap");



    QApplication a(argc, argv);

    MainWindow mainWindow;

    HWND mainWindowHandle = (HWND)mainWindow.winId();

    QString error;
    if (!RyzenAdjSingleton::getInstance().init(error)) {
        //todo make a window for that
        return 1;
    }

    if (!GlobalEventDispatcher::getInstance().init(mainWindowHandle, error)) {
        return 1;
    }
    mainWindow.show();
    return a.exec();
}
