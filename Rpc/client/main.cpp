#include <QCoreApplication>
#include <QDebug>

#include <QTcpSocket>
#include <QHostAddress>

#include <Rpc/proto/request/baserequest.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QObject catcher;

    QTcpSocket *socket = new QTcpSocket();
    QObject::connect(socket, &QTcpSocket::connected, &catcher, [socket]() {
        QByteArray data;
        data.append((char)0x0);
        data.append((char)0x0);
        data.append((char)0x0);
        data.append((char)0x0);
        QByteArray chunk1 = QString("chunk1").toUtf8();
        data.append(chunk1.data(), chunk1.size() + 1);
        data.append((char)0x0);
        data.append((char)0x0);
        auto chunk2 = QString("chunk2").toUtf8();
        data.append(chunk2.data(), chunk2.size() + 1);
        const char chunk3_part1[] = {'c','h','u','n'};
        data.append(chunk3_part1, 4);
        socket->write(data);
        socket->flush();
        data.clear();
        const char chunk3_part2[] = {'k','3', 0x0};
        data.append(chunk3_part2, 3);
        socket->write(data);
        socket->flush();

        //end testing chunks
        Json::FastWriter b;
        auto request1 = g14rpc::BaseRequest();
        auto jsonRequest1 = toJson(request1);
        auto strRequest1 = b.write(jsonRequest1);
        qDebug() << QString::fromStdString(strRequest1);
        socket->write(strRequest1.data(), strRequest1.length() + 1);
        socket->flush();

    });
    socket->connectToHost(QHostAddress::LocalHost, 50022);


    return a.exec();
}
