#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include "Rpc/proto/response/appstateresponse.h"
#include "tcpclient.h"

#include <QObject>

class RpcClient : public QObject
{
    Q_OBJECT
public:
    RpcClient(QObject *parent = nullptr);

    void requestAppState();

private:
    TcpClient *m_tcpClient = nullptr;

    void processResponse(const g14rpc::MessageType type, const QJsonObject &response);

private slots:
    void onCommandResponse(const QByteArray response);
    void onCommandError(const g14rpc::RpcError &error);

signals:
    void appStateResponse(const AppState response);
};

#endif // RPCCLIENT_H
