#include "rpcservercontroller.h"

#include <QCoreApplication>
//#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<g14rpc::AppStateResponse>("g14rpc::AppStateResponse");

    RpcServerController ctrl;
    QString error;
    ctrl.init(error);

    QObject catcher;
    QObject::connect(&ctrl, &RpcServerController::appStateRequested, &catcher, [] (g14rpc::AppStateResponse *response) {
        response->appState.currentPlan = G14ArmouryCratePlan::WindowsPerfomance;
        }, Qt::DirectConnection);


    return a.exec();
}
