#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QDateTime>
#include <QStringList>
#include <QDir>
#include <QSettings>

#include "qtservice.h"

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

#include "main.moc"

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
