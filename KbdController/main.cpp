#include "kbdcontroller.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto *kbdController = new KbdController();

    QString error;

    if (!kbdController->init(error)) {
        qDebug() << error;
    }

    int wait = 100;
    while (true) {
        kbdController->setKbdBrightness(3);
        QThread::msleep(wait);
        kbdController->setKbdBrightness(2);
        QThread::msleep(wait);
        kbdController->setKbdBrightness(1);
        QThread::msleep(wait);
        kbdController->setKbdBrightness(0);
        QThread::msleep(wait);
        kbdController->setKbdBrightness(1);
        QThread::msleep(wait);
        kbdController->setKbdBrightness(2);
        QThread::msleep(wait);
    }

    return a.exec();
}
