#include <QCoreApplication>
#include <QDebug>

#include <Logger/loggersingleton.h>
#include <Rpc/server/rpcservercontroller.h>
#include <Win32Events/win32eventcontroller.h>

#include "controller/servicecontroller.h"


void metatypes()
{
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    QCoreApplication *app = &a;

    QString error;
    Win32EventController* eventController = new Win32EventController();
    if (!eventController->init(error)) {
        qCritical() << error;
        app->quit();
        return -1;
    }

    LoggerSingleton::getInstance().addOutput(eventController, false);

    ServiceController *serviceController = new ServiceController(app);
    if (!serviceController->init(error)) {
        qCritical() << error;
        app->quit();
        return -1;
    } else {
        qDebug() << "Service controller started";
    }

    RpcServerController *rpcController = new RpcServerController(app);
    if (!rpcController->init(error)) {
        qCritical() << error;
        app->quit();
        return -1;
    } else {
        qDebug() << "RPC controller started";
    }

    QObject::connect(rpcController, &RpcServerController::appStateRequested, serviceController, &ServiceController::onAppStateRequested, Qt::DirectConnection);

    return a.exec();
}
