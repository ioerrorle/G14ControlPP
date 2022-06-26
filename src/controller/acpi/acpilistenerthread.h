#ifndef G14CONTROLPP_ACPILISTENERTHREAD_H
#define G14CONTROLPP_ACPILISTENERTHREAD_H

#include <QObject>
#include <QThread>
#include <windows.h>

class AcpiListenerThread : public QThread {
Q_OBJECT
    void run() override;

private:
    HANDLE eventHandle;

public:
    AcpiListenerThread();

signals:
    void acpiEvent(const unsigned long acpiCode);
    void requestAtkAcpiCommand(unsigned long controlCode,
                               unsigned char *inBuffer,
                               int inBufferSize,
                               unsigned char *outBuffer,
                               int outBufferSize);


};


#endif //G14CONTROLPP_ACPILISTENERTHREAD_H
