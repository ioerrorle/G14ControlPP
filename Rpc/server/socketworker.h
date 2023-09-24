#ifndef SOCKETWORKER_H
#define SOCKETWORKER_H

#include <QObject>
#include <QTcpSocket>

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    SocketWorker(QTcpSocket *socket, QObject *parent = nullptr);

public slots:
    void onSocketReadyRead();

private:
    QByteArray m_readBuffer;
    QTcpSocket *m_tcpSocket = nullptr;
    qintptr m_socketDescriptor;

    void processBuffer();

signals:
    QByteArray newRequest(const QByteArray &request);
};

#endif // SOCKETWORKER_H
