#include "rpcservercontroller.h"
#include "socketworker.h"

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
    if (!isListening) {
        error = m_tcpServer->errorString();
        return false;
    }
    return true;
}

QByteArray RpcServerController::processRequest(const g14rpc::MessageType type, const QByteArray &request)
{
    switch (type) {
        case g14rpc::MessageType::APP_STATE:
            return appStateResponse();
            break;
        default:
            return baseErrorResponse(g14rpc::ErrorCode::NOT_IMPLEMENTED, "Command is not implemented");
            break;
        }
}

QByteArray RpcServerController::baseErrorResponse(g14rpc::ErrorCode code, QString message) {
    g14rpc::BaseResponse response;
    response.error.code = code;
    response.error.message = message;

    return toJson(response).toString().toUtf8();
}

QByteArray RpcServerController::appStateResponse()
{
    g14rpc::AppStateResponse response;
    emit appStateRequested(&response);

    return toJson(response).toString().toUtf8();
}

void RpcServerController::onNewTcpConnection()
{
    auto *tcpSocket = m_tcpServer->nextPendingConnection();
    auto *socketWorker = new SocketWorker(tcpSocket);
    QThread *thread = tcpSocket->thread();
    socketWorker->moveToThread(thread);
    connect(thread, &QThread::finished, socketWorker, &SocketWorker::deleteLater);
    connect(socketWorker, &SocketWorker::newRequest, this, &RpcServerController::onNewSocketRequest, Qt::BlockingQueuedConnection);
}

QByteArray RpcServerController::onNewSocketRequest(const QByteArray request)
{
    QJsonParseError error;
    QJsonDocument a = QJsonDocument::fromJson(request, &error);
    if (error.error == QJsonParseError::NoError) {
        if (!a.isObject()) {
            qDebug() << "Doc is not an object";
            return baseErrorResponse(g14rpc::ErrorCode::NOT_IMPLEMENTED, "JSON is not an object");
        } else {
            g14rpc::BaseRequest baseRequest = fromJson<g14rpc::BaseRequest>(a.object());
            return processRequest(baseRequest.type, request);
        }
    } else {
        qDebug() << "couldn't parse message" << error.errorString();
        return baseErrorResponse(g14rpc::ErrorCode::NOT_IMPLEMENTED, "Coldn't parse message: " + error.errorString());
    }
}
