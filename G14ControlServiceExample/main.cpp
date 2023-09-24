#include <QCoreApplication>
#include <QDebug>

#include <Logger/loggersingleton.h>
#include <Rpc/server/rpcservercontroller.h>
#include <Win32Events/win32eventcontroller.h>

#include "controller/servicecontroller.h"

void start()
{
    QCoreApplication *app = application();

    QString error;
    Win32EventController *eventController = new Win32EventController();
    if (!eventController->init(error)) {
        qCritical() << error;
        app->quit();
        return;
    }

    LoggerSingleton::getInstance().addOutput(eventController, false);

    ServiceController *serviceController = new ServiceController(app);
    if (!serviceController->init(error)) {
        qCritical() << error;
        app->quit();
        return;
    } else {
        qDebug() << "Service controller started";
    }

    RpcServerController *rpcController = new RpcServerController(app);
    if (!rpcController->init(error)) {
        qCritical() << error;
        app->quit();
        return;
    } else {
        qDebug() << "RPC controller started";
    }

    QObject::connect(rpcController, &RpcServerController::appStateRequested, serviceController, &ServiceController::onAppStateRequested, Qt::DirectConnection);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}


