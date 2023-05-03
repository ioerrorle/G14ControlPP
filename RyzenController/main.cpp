#include <cpureader.h>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString error;
    CpuReader reader(&a);
    if (!reader.init(error)) {
        qCritical().noquote() << error;
        return -1;
    }

    CpuState state;
    if (!reader.getCpuState(state, error)) {
        qCritical().noquote() << error;
        return -1;
    }

    qInfo().noquote() << "Core temp limit is" << state.coreTempLimit;

    return a.exec();
}
