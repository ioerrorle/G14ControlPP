#include <QCoreApplication>
#include <QDebug>

#include "tcpdaemon.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpDaemon daemon;
    daemon.startListening(40000);

    QObject catcher;

    QObject::connect(&daemon, &TcpDaemon::newMessage, &catcher, [](QByteArray &message) {
       qDebug() << message;
    });

    return a.exec();
}
