//
// Created by ioerror on 28.09.2021.
//

#ifndef G14CONTROLPP_KBDLISTENERTHREAD_H
#define G14CONTROLPP_KBDLISTENERTHREAD_H

#include <QObject>
#include <QDebug>
#include <QLibrary>
#include <QThread>
#include <windows.h>
#include <initguid.h>
#include <usbiodef.h>
#include <hidclass.h>
#include <cfgmgr32.h>
#include <hidsdi.h>
#include <src/atkacpi/AcpiControlSingleton.h>

typedef WINBOOL (*HidD_SetFeature_Fn)(void*, void*, unsigned long);

class KbdListenerThread : public QThread {
Q_OBJECT
    void run() override;

private:
    QVector<HANDLE> *kbdEventHandles;
    QVector<HANDLE> *kbdHandles;
    QVector<LPOVERLAPPED> *kbdOverlaps;
    QVector<LPVOID> *kbdReadResults;
    QVector<LPDWORD> *kbdReadCount;
    QVector<INPUT> *pressedKeys;

    void createReadFileJob(QString &qString);
    void sendInputIfNeeded(const unsigned char * bytes, int nCount);
    void sendScanCode(WORD hwScanCode, WORD vScanCode);
    void releaseKey();

    void sendHidControl(unsigned char * data);
    void changeKbdBrightness(bool increase);
    unsigned char kbdBr = 0;
    HidD_SetFeature_Fn HidD_SetFeature_Ptr = NULL;

signals:

    void resultReady(const QString &s);

public:
    KbdListenerThread(QString &error);


};


#endif //G14CONTROLPP_KBDLISTENERTHREAD_H
