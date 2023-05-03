#include "socketworker.h"
#include <QDebug>

SocketWorker::SocketWorker(QTcpSocket *tcpSocket, QObject *parent)
    : QObject{parent}
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &SocketWorker::onSocketReadyRead);
    connect(tcpSocket, &QTcpSocket::connected, this, &SocketWorker::onSocketConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &SocketWorker::onSocketDisconnected);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &SocketWorker::onSocketErrorOccured);
    connect(tcpSocket, &QTcpSocket::hostFound, this, &SocketWorker::onSocketHostFound);
    connect(tcpSocket, &QTcpSocket::stateChanged, this, &SocketWorker::onSocketStateChanged);
    connect(tcpSocket, &QTcpSocket::aboutToClose, this, &SocketWorker::onSocketAboutToClose);
}

void SocketWorker::onSocketReadyRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
//    socket->write("\u001B[2J");
//    socket->flush();
    QByteArray message;
    char byte;
    //while (true) {
        qDebug() << socket->bytesAvailable();
    //}
        qDebug() << "Exit read client";
}

void SocketWorker::onSocketConnected()
{
    qDebug() << "Connected";
}

void SocketWorker::onSocketDisconnected()
{
    qDebug() << "Disconnected";
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void SocketWorker::onSocketErrorOccured(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error" << error;
}

void SocketWorker::onSocketHostFound()
{
    qDebug() << "Host found";
}

void SocketWorker::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Socket state changed" << state;
}

void SocketWorker::onSocketAboutToClose()
{
    qDebug() << "Socket about to close";
}
