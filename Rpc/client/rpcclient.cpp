#include "rpcclient.h"

RpcClient::RpcClient(QObject *parent)
    : QObject{parent}
{

}

bool RpcClient::connectToHost(const QHostAddress &address, quint16 port, QString error)
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

    connect(m_socket, &QTcpSocket::stateChanged, this, &RpcClient::onSocketStateChanged);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &RpcClient::onSocketErrorOccured);

    connect(m_socket, &QTcpSocket::bytesWritten, this, &RpcClient::onSocketBytesWritten);

    connect(m_socket, &QTcpSocket::readyRead, this, &RpcClient::onSocketReadyRead);

    m_thread->start();

    m_socket->connectToHost(address, port);
}

void RpcClient::sendCommand(const QByteArray &request)
{
    qint64 bytesWritten = m_socket->write(request);
    if (bytesWritten != request.size()) {
        g14rpc::RpcError error;
        error.code = g14rpc::ErrorCode::SEND_FAILED;
        error.message = tr("Couldn't send message, message size = %1, bytes written = %2")
                .arg(request.size())
                .arg(bytesWritten);

        emit commandErrorOccured(error);
    }
}

void RpcClient::processBuffer()
{
    qDebug() << QString::fromUtf8(m_buffer);
    m_buffer.clear();
}

void RpcClient::onSocketBytesWritten(qint64 bytes)
{

}

void RpcClient::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState) {
        //deleting socket
        QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
        if (socket == nullptr)
            return;
        emit socket->disconnected();
    }
}

void RpcClient::onSocketErrorOccured(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}

void RpcClient::onSocketReadyRead()
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
