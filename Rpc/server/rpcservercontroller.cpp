#include "rpcservercontroller.h"

#include <Rpc/proto/request/baserequest.h>

RpcServerController::RpcServerController(QObject *parent)
    : QObject{parent}
{

}

bool RpcServerController::init(QString &error)
{
    m_tcpServer = new RpcServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &RpcServerController::onNewTcpConnection);

    bool isListening = m_tcpServer->listen(QHostAddress::LocalHost, 50022);
    if (!isListening)
        qDebug() << "error" << m_tcpServer->errorString();
    qDebug() << isListening;
}

void RpcServerController::processBuffer()
{
    //trying to parse it as json
    Json::CharReaderBuilder r;
    auto * reader = r.newCharReader();
    try {
        Json::Value val;
        Json::String errs;
        const char *start = m_buffer.constData();
        const char *end = &m_buffer.constData()[m_buffer.size() - 1];
        bool ok = reader->parse(start, end, &val, &errs);
        if (ok) {
            g14rpc::BaseRequest baseRequest = fromJson<g14rpc::BaseRequest>(val);
            qDebug() << "messageType" << (int)baseRequest.type;
        } else {
            qDebug() << "couldn't parse message";
        }
    } catch (Json::Exception e) {
        qDebug() << "error parsing message" << e.what();
    }
    m_buffer.clear();
    delete reader;
}

void RpcServerController::onNewTcpConnection()
{
//    if (m_tcpSocket != nullptr) {
//        m_tcpSocket->close();
//    }
    m_buffer.clear();
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    m_socketDescriptor = m_tcpSocket->socketDescriptor();
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &RpcServerController::onSocketReadyRead);
}

void RpcServerController::onSocketReadyRead()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    if (socket == nullptr)
        return;
    if (socket->socketDescriptor() != m_socketDescriptor)
        return;

    int nullIndex = -1;
    QByteArray socketData = socket->readAll();
    while ((nullIndex = socketData.indexOf((char)0x0)) != -1) {
        QByteArray messageChunk = socketData.mid(0, nullIndex + 1);
        socketData.remove(0, nullIndex + 1);
        if (messageChunk.size() == 0 || messageChunk.at(0) == 0)
            continue;
        m_buffer.append(messageChunk);
        m_buffer.append((char)0x0);
        processBuffer();
    }
    m_buffer.append(socketData);
}
