#ifndef RPCSERVERCONTROLLER_H
#define RPCSERVERCONTROLLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "Rpc/proto/response/appstateresponse.h"
#include "rpcserver.h"

class RpcServerController : public QObject
{
    Q_OBJECT
public:
    explicit RpcServerController(QObject *parent = nullptr);
    ~RpcServerController();
    bool init(QString &error);

private:
    RpcServer *m_tcpServer = nullptr;

    QByteArray processRequest(const g14rpc::MessageType type, const QByteArray &request);

    QByteArray baseErrorResponse(g14rpc::ErrorCode, QString message);
    QByteArray appStateResponse();

private slots:
    void onNewTcpConnection();

    QByteArray onNewSocketRequest(const QByteArray request);

signals:
    void socketWriteRequested(const QByteArray data);

    void appStateRequested(g14rpc::AppStateResponse *response);


};

#endif // RPCSERVERCONTROLLER_H
