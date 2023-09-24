#include "rpcclient.h"
#include <QCoreApplication>
#include <QDebug>

#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonDocument>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    QObject catcher;

    RpcClient c;

    QObject::connect(&c, &RpcClient::appStateResponse, &catcher, [](const g14rpc::AppStateResponse response) {
        qDebug() << (int)response.appState.currentPlan;
    });

    c.requestAppState();


    return a.exec();
}
