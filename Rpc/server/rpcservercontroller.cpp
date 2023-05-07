#include "rpcservercontroller.h"

#include <Rpc/proto/request/baserequest.h>

#include <QJsonDocument>

RpcServerController::RpcServerController(QObject *parent)
    : QObject{parent}
{

}

RpcServerController::~RpcServerController()
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
    return true;
}

void RpcServerController::processBuffer()
{
    //trying to parse it as json
    QJsonParseError error;
    QJsonDocument a = QJsonDocument::fromJson(m_buffer, &error);
    if (error.error == QJsonParseError::NoError) {
        if (!a.isObject()) {
            qDebug() << "Doc is not an object";
        } else {
            g14rpc::BaseRequest baseRequest = fromJson<g14rpc::BaseRequest>(a.object());
        }
    } else {
        qDebug() << "couldn't parse message" << error.errorString();
    }

    m_buffer.clear();
}

void RpcServerController::onNewTcpConnection()
{
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
        QByteArray messageChunk = socketData.mid(0, nullIndex);
        socketData.remove(0, nullIndex + 1);
        if (messageChunk.size() == 0 || messageChunk.at(0) == 0)
            continue;
        m_buffer.append(messageChunk);
        //m_buffer.append((char)0x0);
        processBuffer();
    }
    m_buffer.append(socketData);
}
