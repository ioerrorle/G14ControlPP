#ifndef TCPDAEMON_H
#define TCPDAEMON_H

#include <QTcpServer>

class TcpDaemon : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpDaemon(QObject *parent = nullptr);
    void startListening(quint16 port);
    void stopListening();

private:
    static const qint64 READ_BUFFER_SIZE = 65536;

    QMap<QTcpSocket*, QByteArray> m_buffers;

protected:
    void incomingConnection(qintptr handle) override;

private slots:
    void onSocketReadyRead();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketErrorOccured(QAbstractSocket::SocketError error);
    void onSocketHostFound();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void onSocketAboutToClose();

signals:
    void newMessage(QByteArray &message);
};

#endif // TCPDAEMON_H
