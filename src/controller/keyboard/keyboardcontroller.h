#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "src/ui/hotkeys/HotkeysProfile.h"
#include "src/controller/keyboard/kbdlistenerthread.h"
#include <QObject>
#include <QString>
#include <windows.h>
#include <hidsdi.h>
#include <initguid.h>
#include <hidclass.h>
#include <cfgmgr32.h>

//typedef WINBOOL (*HidD_SetFeature_Fn)(void*, void*, unsigned long);

class KeyboardController : public QObject
{
    Q_OBJECT
public:
    KeyboardController(HotkeysProfile *profile, QObject *parent = nullptr);
    bool init(QString &error);

public slots:
    void onFnKeyPressed(const unsigned char fnKeyCode);
    void setKbdBrightness(const unsigned char brightness);

private:
    HotkeysProfile *profile;
    INPUT *pressedKey;
    //HidD_SetFeature_Fn HidD_SetFeature_Ptr = nullptr;
    HANDLE kbdHandle = nullptr;
    KbdListenerThread *kbdListenerThread = nullptr;
    HANDLE createKbdHandle(const QString &devicePath);
    void processFnKeyAction(const HotkeyAction &action);
    void sendScanCode(quint32 hwScanCode, quint32 modifiers);
    void releaseKey();
    void sendHidControl(unsigned char *data, unsigned short size);
};

#endif // KEYBOARDCONTROLLER_H
