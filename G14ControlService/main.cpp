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

//        const QStringList arguments = QCoreApplication::arguments();
//        quint16 port = (arguments.size() > 1) ?
//                arguments.at(1).toUShort() : 8080;
//        daemon = new HttpDaemon(port, app);

//        if (!daemon->isListening()) {
//            logMessage(QString("Failed to bind to port %1").arg(daemon->serverPort()), QtServiceBase::Error);
//            app->quit();
//        }
    }

    void pause()
    {
    //daemon->pause();
    }

    void resume()
    {
    //daemon->resume();
    }

//private:
    //HttpDaemon *daemon;
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
