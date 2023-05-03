#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <QTcpServer>
#include <QThread>

class RpcServer : public QTcpServer
{
public:
    explicit RpcServer(QObject *parent = nullptr);



    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle);

private slots:
    void onThreadDestroyed();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onSocketErrorOccured(QAbstractSocket::SocketError socketError);
};

#endif // RPCSERVER_H
