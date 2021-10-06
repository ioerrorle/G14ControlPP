#include <QApplication>
#include <windows.h>
#include <src/ui/MainWindow.h>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/events/GlobalEventDispatcher.h>

int main(int argc, char *argv[]) {
    qRegisterMetaTypeStreamOperators<_FanCurve>("_FanCurve");
    qRegisterMetaTypeStreamOperators<_FansProfile>("_FansProfile");
    qRegisterMetaTypeStreamOperators<QList<_FanCurve>>("_FanCurveList");
    qRegisterMetaTypeStreamOperators<QList<_FansProfile>>("_FansProfileList");

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
