#ifndef G14CONTROLPP_ACPILISTENERTHREAD_H
#define G14CONTROLPP_ACPILISTENERTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <windows.h>

class ACPIListenerThread : public QThread {
    Q_OBJECT
    void run() override;

private:

    HANDLE acpiHandle;
    HANDLE eventHandle;

    signals:
            void resultReady(const QString &s);

public:
    ACPIListenerThread(HANDLE acpiHandle, QString &error);
};


#endif //G14CONTROLPP_ACPILISTENERTHREAD_H
