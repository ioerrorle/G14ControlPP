#ifndef G14CONTROLPP_KBDLISTENERTHREAD_H
#define G14CONTROLPP_KBDLISTENERTHREAD_H

#include <QObject>
#include <QThread>
#include <windows.h>
#include <hidsdi.h>

class KbdListenerThread : public QThread {
Q_OBJECT
    void run() override;

private:
    HANDLE *kbdHandle;
    HANDLE kbdEventHandle;
    LPOVERLAPPED kbdOverlap;
    LPVOID kbdReadResult;
    LPDWORD kbdReadCount;

    void emitKeyPress(const unsigned char * bytes, int nCount);

public:
    KbdListenerThread(HANDLE *device, QString &error);

signals:
    void fnKeyPressCaptured(const unsigned char fnKeyCode);
    void error(int winCode, QString errorMessage);
};


#endif //G14CONTROLPP_KBDLISTENERTHREAD_H
