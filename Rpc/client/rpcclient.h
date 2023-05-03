#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <QMutex>
#include <QObject>
#include <QTcpSocket>
#include <QThread>

#include <Rpc/proto/response/error.h>

class RpcClient : public QObject
{
    Q_OBJECT
public:
    explicit RpcClient(QObject *parent = nullptr);
    bool connectToHost(const QHostAddress &address, quint16 port, QString error);
    void sendCommand(const QByteArray &request);

private:
    QTcpSocket *m_socket = nullptr;
    QThread *m_thread = nullptr;

    QByteArray m_buffer;

    void processBuffer();

private slots:
    void onSocketBytesWritten(qint64 bytes);
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    void onSocketErrorOccured(QAbstractSocket::SocketError socketError);
    void onSocketReadyRead();

signals:
    void commandErrorOccured(const g14rpc::RpcError &error);

};

#endif // RPCCLIENT_H
