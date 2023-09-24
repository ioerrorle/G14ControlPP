#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMutex>
#include <QObject>
#include <QTcpSocket>
#include <QThread>

#include <Rpc/proto/response/error.h>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    bool connectToHost(const QHostAddress &address, quint16 port, QString error);

    template<typename T>
    void sendCommand(const T &request)
    {
        QJsonValue requestJson = toJson(request);
        QJsonDocument requestDoc(requestJson.toObject());
        QByteArray bytesRequest = QByteArray(requestDoc.toJson().constData(), requestDoc.toJson().size() + 1);

        QMetaObject::invokeMethod(m_socket, [=]{
                qint64 bytesWritten = m_socket->write(bytesRequest);
                if (bytesWritten != bytesRequest.size()) {
                    g14rpc::RpcError error;
                    error.code = g14rpc::ErrorCode::SEND_FAILED;
                    error.message = tr("Couldn't send message, message size = %1, bytes written = %2")
                                        .arg(bytesRequest.size())
                                        .arg(bytesWritten);

                    emit commandErrorOccured(error);
                }
            }, Qt::QueuedConnection);
    }

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
    void commandResponse(const QByteArray response);
    void commandErrorOccured(const g14rpc::RpcError &error);

};

#endif // TCPCLIENT_H
