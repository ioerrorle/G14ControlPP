#include "socketworker.h"

SocketWorker::SocketWorker(QTcpSocket *socket, QObject *parent)
    : QObject{parent}
    , m_tcpSocket(socket)
{
    m_socketDescriptor = m_tcpSocket->socketDescriptor();
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &SocketWorker::onSocketReadyRead);
}

void SocketWorker::onSocketReadyRead()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    if (socket == nullptr)
        return;
    if (socket->socketDescriptor() != m_socketDescriptor)
        return;

    int nullIndex = -1;
    QByteArray socketData = socket->readAll();
    while ((nullIndex = socketData.indexOf((char)0x0)) != -1) {
        QByteArray messageChunk = socketData.mid(0, nullIndex);
        socketData.remove(0, nullIndex + 1);
        if (messageChunk.size() == 0 || messageChunk.at(0) == 0)
            continue;
        m_readBuffer.append(messageChunk);
        processBuffer();
    }
    m_readBuffer.append(socketData);
}

void SocketWorker::processBuffer()
{
    QByteArray response = emit newRequest(m_readBuffer);
    m_tcpSocket->write(response);
    m_readBuffer.clear();
}
