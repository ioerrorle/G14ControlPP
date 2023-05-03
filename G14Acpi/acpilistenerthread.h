#ifndef ACPILISTENERTHREAD_H
#define ACPILISTENERTHREAD_H

#include <QObject>
#include <QThread>
#include <windows.h>

class AcpiListenerThread : public QThread
{
    Q_OBJECT
public:
    AcpiListenerThread(const HANDLE atkAcpiHandle, QObject *parent = nullptr);
    ~AcpiListenerThread();
    bool init(QString &error);

protected:
    void run() override;

private:
    const HANDLE m_atkAcpiHandle;
    HANDLE m_eventHandle = NULL;
    HANDLE m_quitEventHandle = NULL;

    bool processWaitResults(const DWORD waitResult, const DWORD handleCount, const int exitHandleIndex, bool &shouldExit);

signals:
    void acpiEvent(const QByteArray &data);
    void acpiEventError(const QString &error);
};

#endif // ACPILISTENERTHREAD_H
