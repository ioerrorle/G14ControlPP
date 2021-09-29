#include <QApplication>
#include <QDebug>
#include <src/ui/MainWindow.h>
#include <src/ryzenadj/RyzenAdjSingleton.h>
#include <src/atkacpi/AcpiControlSingleton.h>
#include <src/kbdlistener/KbdListenerSingleton.h>

int main(int argc, char *argv[])
{
    QString error = nullptr;
    if (!RyzenAdjSingleton::getInstance().init(error)) {
        //todo make a window for that
        return 1;
    }

    if (!AcpiControlSingleton::getInstance().init()) {
        //todo make a window for that
        return 1;
    }

    if (!KbdListenerSingleton::getInstance().init()) {
        return 1;
    } else {
        KbdListenerSingleton::getInstance().startListening();
    }

    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();




    return a.exec();
}
