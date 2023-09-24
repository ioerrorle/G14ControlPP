#include "tcpclient.h"
#include <QHostAddress>

TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{

}

bool TcpClient::connectToHost(const QHostAddress &address, quint16 port, QString error)
{
    m_thread = new QThread();
    m_socket = new QTcpSocket();
    m_socket->moveToThread(m_thread);

    connect(m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);
    connect(m_socket, &QTcpSocket::destroyed, this, [=]() {
        qDebug() << "Socket destroyed";
    });
    connect(m_socket, &QTcpSocket::disconnected, m_thread, &QThread::quit);
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(m_thread, &QThread::destroyed, this, [=]() {
        qDebug() << "Thread destroyed";
    });

    connect(m_socket, &QTcpSocket::stateChanged, this, &TcpClient::onSocketStateChanged);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &TcpClient::onSocketErrorOccured);

    connect(m_socket, &QTcpSocket::bytesWritten, this, &TcpClient::onSocketBytesWritten);

    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onSocketReadyRead);

    connect(m_thread, &QThread::started, m_socket, [=]{
        m_socket->connectToHost(address, port);
    });

    m_thread->start();

    return true;
}

void TcpClient::processBuffer()
{
    emit commandResponse(m_buffer);
    m_buffer.clear();
}

void TcpClient::onSocketBytesWritten(qint64 bytes)
{

}

void TcpClient::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState) {
        //deleting socket
        QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
        if (socket == nullptr)
            return;
        emit socket->disconnected();
    }
}

void TcpClient::onSocketErrorOccured(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}

void TcpClient::onSocketReadyRead()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    if (socket == nullptr)
        return;

    int nullIndex = -1;
    QByteArray socketData = socket->readAll();
    while ((nullIndex = socketData.indexOf((char)0x0)) != -1) {
        QByteArray messageChunk = socketData.mid(0, nullIndex);
        socketData.remove(0, nullIndex + 1);
        if (messageChunk.size() == 0)
            continue;
        m_buffer.append(messageChunk);
        processBuffer();
    }
    m_buffer.append(socketData);
}
