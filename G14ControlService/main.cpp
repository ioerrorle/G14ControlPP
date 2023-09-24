#include <QCoreApplication>
#include <QDebug>

#include <Logger/loggersingleton.h>
#include <Rpc/server/rpcservercontroller.h>
#include <Win32Events/win32eventcontroller.h>
#include <qtservice.h>

#include "controller/servicecontroller.h"


class G14ControlPpService : public QtService<QCoreApplication>
{
public:
    G14ControlPpService(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "G14ControlPP service")
    {
        qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
        qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

        setServiceDescription("Controls power profiles of G14 laptop");
        setServiceFlags(QtServiceBase::CanBeSuspended);
    }

protected:
    void start()
    {
        QCoreApplication *app = application();

        QString error;
        Win32EventController* eventController = new Win32EventController();
        if (!eventController->init(error)) {
            qCritical() << error;
            app->quit();
            return;
        }

        LoggerSingleton::getInstance().addOutput(eventController, false);

        ServiceController *serviceController = new ServiceController(app);
        if (!serviceController->init(error)) {
            qCritical() << error;
            app->quit();
            return;
        } else {
            qDebug() << "Service controller started";
        }

        RpcServerController *rpcController = new RpcServerController(app);
        if (!rpcController->init(error)) {
            qCritical() << error;
            app->quit();
            return;
        } else {
            qDebug() << "RPC controller started";
        }

        QObject::connect(rpcController, &RpcServerController::appStateRequested, serviceController, &ServiceController::onAppStateRequested, Qt::DirectConnection);
    }

    void pause()
    {

    }

    void resume()
    {

    }
};

//#include "main.moc"

int main(int argc, char **argv)
{
#if !defined(Q_OS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    G14ControlPpService service(argc, argv);
    return service.exec();
}
