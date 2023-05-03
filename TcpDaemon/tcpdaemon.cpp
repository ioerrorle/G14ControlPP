#include "tcpdaemon.h"

#include <QTcpSocket>

TcpDaemon::TcpDaemon(QObject *parent)
    : QTcpServer{parent}
{

}

void TcpDaemon::startListening(quint16 port)
{
    if (!isListening())
        listen(QHostAddress::Any, port);
}

void TcpDaemon::stopListening()
{
    if (isListening())
        close();
}

void TcpDaemon::incomingConnection(qintptr handle)
{
    qDebug() << "New connection";
    QTcpSocket* tcpSocket = new QTcpSocket(this);
    tcpSocket->setReadBufferSize(READ_BUFFER_SIZE);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpDaemon::onSocketReadyRead);
    connect(tcpSocket, &QTcpSocket::connected, this, &TcpDaemon::onSocketConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpDaemon::onSocketDisconnected);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &TcpDaemon::onSocketErrorOccured);
    connect(tcpSocket, &QTcpSocket::hostFound, this, &TcpDaemon::onSocketHostFound);
    connect(tcpSocket, &QTcpSocket::stateChanged, this, &TcpDaemon::onSocketStateChanged);
    connect(tcpSocket, &QTcpSocket::aboutToClose, this, &TcpDaemon::onSocketAboutToClose);
    tcpSocket->setSocketDescriptor(handle);
    addPendingConnection(tcpSocket);
    qDebug() << handle;
}

void TcpDaemon::onSocketReadyRead()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QByteArray *buffer = &m_buffers[socket];
    char byte;
    while (socket->getChar(&byte)) {
        buffer->append(byte);
        if (byte == 0x00 || byte == 0x03 || byte == 0x04) {
            emit newMessage(*buffer);
            buffer->clear();
        }
    }
    qDebug() << "Exit onSocketReadyRead";
}

void TcpDaemon::onSocketConnected()
{
    qDebug() << "Connected";
}

void TcpDaemon::onSocketDisconnected()
{
    qDebug() << "Disconnected";
    QTcpSocket* socket = (QTcpSocket*)sender();
    m_buffers.remove(socket);
    socket->deleteLater();
}

void TcpDaemon::onSocketErrorOccured(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error" << error;
    QTcpSocket* socket = (QTcpSocket*)sender();
    (&m_buffers[socket])->clear();
}

void TcpDaemon::onSocketHostFound()
{
    qDebug() << "Host found";
}

void TcpDaemon::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Socket state changed" << state;
}

void TcpDaemon::onSocketAboutToClose()
{
    qDebug() << "Socket about to close";
}


