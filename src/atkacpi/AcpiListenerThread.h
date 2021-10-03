#ifndef G14CONTROLPP_ACPILISTENERTHREAD_H
#define G14CONTROLPP_ACPILISTENERTHREAD_H

#include <QObject>
#include <QThread>
//#include <QDebug>
#include <windows.h>
//#include <winuser.h>

class AcpiListenerThread : public QThread {
Q_OBJECT

    void run() override;

private:
    HANDLE acpiHandle;
    HANDLE eventHandle;

public:
    AcpiListenerThread(HANDLE acpiHandle, QString &error);

signals:
    void resultReady(const unsigned long acpiCode);
};


#endif //G14CONTROLPP_ACPILISTENERTHREAD_H
