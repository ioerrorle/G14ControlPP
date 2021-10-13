#ifndef G14CONTROLPP_KBDCONTROLSINGLETON_H
#define G14CONTROLPP_KBDCONTROLSINGLETON_H

#include <QObject>
#include <QLibrary>
#include "windows.h"
#include <initguid.h>
#include <hidclass.h>
#include <cfgmgr32.h>
#include "KbdListenerThread.h"

typedef WINBOOL (*HidD_SetFeature_Fn)(void*, void*, unsigned long);

class KbdControlSingleton : public QObject {
Q_OBJECT
public:
    static KbdControlSingleton &getInstance();

private:
    KbdControlSingleton();

    HidD_SetFeature_Fn HidD_SetFeature_Ptr = NULL;
    HANDLE kbdHandle = nullptr;
    KbdListenerThread *kbdListenerThread;
    unsigned char kbdBr;

    HANDLE createKbdHandle(const QString &qString);
    void sendHidControl(unsigned char *data, unsigned short size);

public:
    bool init(QString &error);

    void setKbdBrightness(uchar &kbdBr);
    uchar changeKbdBrightness(bool increase);

    void toggleKbdBacklight(bool enabled);
    void toggleTouchPad();

public slots:
    void handleKbdFnPress(const unsigned char fnKeyCode);

signals:
    void kbdFnEvent(const unsigned char fnKeyCode);

    void sendHidControl(unsigned char *data);
};


#endif //G14CONTROLPP_KBDCONTROLSINGLETON_H
