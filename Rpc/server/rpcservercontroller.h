#ifndef RPCSERVERCONTROLLER_H
#define RPCSERVERCONTROLLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "rpcserver.h"

class RpcServerController : public QObject
{
    Q_OBJECT
public:
    explicit RpcServerController(QObject *parent = nullptr);
    bool init(QString &error);

private:
    RpcServer *m_tcpServer = nullptr;

    QByteArray m_buffer;
    QTcpSocket *m_tcpSocket = nullptr;
    qintptr m_socketDescriptor;
    void processBuffer();

private slots:
    void onNewTcpConnection();
    void onSocketReadyRead();

signals:

};

#endif // RPCSERVERCONTROLLER_H
