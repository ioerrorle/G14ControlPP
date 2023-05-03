#ifndef SOCKETWORKER_H
#define SOCKETWORKER_H

#include <QObject>
#include <QTcpSocket>

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    explicit SocketWorker(QTcpSocket *socket, QObject *parent = nullptr);

private slots:
    void onSocketReadyRead();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketErrorOccured(QAbstractSocket::SocketError error);
    void onSocketHostFound();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void onSocketAboutToClose();

};

#endif // SOCKETWORKER_H
