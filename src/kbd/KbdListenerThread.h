#ifndef G14CONTROLPP_KBDLISTENERTHREAD_H
#define G14CONTROLPP_KBDLISTENERTHREAD_H

#include <QObject>
//#include <QDebug>
//#include <QLibrary>
#include <QThread>
#include <windows.h>
//#include <initguid.h>
//#include <usbiodef.h>
//#include <hidclass.h>
//#include <cfgmgr32.h>
//#include <hidsdi.h>

typedef WINBOOL (*HidD_SetFeature_Fn)(void*, void*, unsigned long);

class KbdListenerThread : public QThread {
Q_OBJECT
    void run() override;

private:

    HANDLE *kbdHandle;
    HANDLE kbdEventHandle;
    LPOVERLAPPED kbdOverlap;
    LPVOID kbdReadResult;
    LPDWORD kbdReadCount;
    //QVector<INPUT> *pressedKeys;

    void emitKeyPress(const unsigned char * bytes, int nCount);
    //void sendScanCode(WORD hwScanCode, WORD vScanCode);
    //void releaseKey();

    //void sendHidControl(unsigned char * data);
    //void changeKbdBrightness(bool increase);
    //unsigned char kbdBr = 0;
    //HidD_SetFeature_Fn HidD_SetFeature_Ptr = NULL;

public:
    KbdListenerThread(HANDLE *device, QString &error);

signals:
    void resultReady(const unsigned char fnKeyCode);
};


#endif //G14CONTROLPP_KBDLISTENERTHREAD_H
