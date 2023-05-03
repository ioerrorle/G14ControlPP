#include "g14acpi.h"

#include <QCoreApplication>
#include <QDebug>

using G14AcpiCodes::Function;
using G14AcpiCodes::CommandCode;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto ctrl = new G14Acpi();

    QString error;
    if (!ctrl->initAtkAcpi(error))
        qDebug() << error;

    QByteArray result(0x10, 0x01);
    for (int i = 0; i < 10; ++i) {
        qDebug().noquote() << i;

        ctrl->sendDeviceIoControl(Function::DSTS, CommandCode::x00110022, result, error);
        qDebug().noquote() << result.toHex(' ');
        ctrl->sendDeviceIoControl(Function::DSTS, CommandCode::x00110023, result, error);
        qDebug().noquote() << result.toHex(' ');
        result.fill(0x0);

        QThread::currentThread()->msleep(500);
    }


    return a.exec();
}
