#include "rpcserver.h"

#include <QTcpSocket>

RpcServer::RpcServer(QObject *parent)
    : QTcpServer{parent}
{

}

void RpcServer::incomingConnection(qintptr socketDescriptor)
{
    //making thread for socket
    qDebug() << "Creating socket and thread" << socketDescriptor;
    QThread *thread = new QThread();
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);
    socket->moveToThread(thread);

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(socket, &QTcpSocket::destroyed, this, [=]() {
        qDebug() << "Socket destroyed" << socketDescriptor;
    });
    connect(socket, &QTcpSocket::disconnected, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::destroyed, this, [=]() {
        qDebug() << "Thread destroyed" << socketDescriptor;
    });

    connect(socket, &QTcpSocket::stateChanged, this, &RpcServer::onSocketStateChanged);
    connect(socket, &QTcpSocket::errorOccurred, this, &RpcServer::onSocketErrorOccured);

    thread->start();

    addPendingConnection(socket);
}

void RpcServer::onThreadDestroyed()
{

}

void RpcServer::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState) {
        //deleting socket
        QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
        if (socket == nullptr)
            return;
        emit socket->disconnected();
    }
}

void RpcServer::onSocketErrorOccured(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}
