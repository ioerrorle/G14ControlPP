#include "rpcclient.h"
#include "Rpc/proto/request/appstaterequest.h"
#include "Rpc/proto/response/baseresponse.h"

#include <QHostAddress>
#include <QJsonDocument>

RpcClient::RpcClient(QObject *parent)
    : QObject{parent}
    , m_tcpClient(new TcpClient(this))
{
    connect(m_tcpClient, &TcpClient::commandResponse, this, &RpcClient::onCommandResponse);
    connect(m_tcpClient, &TcpClient::commandErrorOccured, this, &RpcClient::onCommandError);

    QString error;
    m_tcpClient->connectToHost(QHostAddress::LocalHost, 50022, error);
}

void RpcClient::requestAppState()
{
    g14rpc::AppStateRequest request;
    m_tcpClient->sendCommand(request);
}

void RpcClient::processResponse(const g14rpc::MessageType type, const QJsonObject &response)
{
    switch (type) {
        case g14rpc::MessageType::APP_STATE:
        {
            g14rpc::AppStateResponse resp = fromJson<g14rpc::AppStateResponse>(response);
            emit appStateResponse(resp.appState);
            break;
        }
        default:
            break;
    }
}

void RpcClient::onCommandResponse(const QByteArray response)
{
    QJsonParseError jsonParseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &jsonParseError);
    if (jsonParseError.error == QJsonParseError::NoError) {
        if (!jsonDoc.isObject()) {
            qDebug() << "Doc is not an object";
        } else {
            g14rpc::BaseResponse baseResponse = fromJson<g14rpc::BaseResponse>(jsonDoc.object());
            processResponse(baseResponse.type, jsonDoc.object());
        }
    }
}

void RpcClient::onCommandError(const g14rpc::RpcError &error)
{

}
