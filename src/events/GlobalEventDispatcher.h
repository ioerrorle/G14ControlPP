#ifndef G14CONTROLPP_GLOBALEVENTDISPATCHER_H
#define G14CONTROLPP_GLOBALEVENTDISPATCHER_H

#include <QAbstractNativeEventFilter>
#include <QAbstractEventDispatcher>
#include <QDebug>
#include <windows.h>
#include <QObject>
#include <QtCore/QAbstractEventDispatcher>
#include "src/kbd/KbdControlSingleton.h"
#include "src/atkacpi/AcpiControlSingleton.h"
//#include "src/kbd/KbdListenerThread.h"

class GlobalEventDispatcher : public QObject, public QAbstractNativeEventFilter {
Q_OBJECT

public:
    static GlobalEventDispatcher &getInstance();

private:
    GlobalEventDispatcher();

    INPUT *pressedKey;

    void handlePowerCfgChange(MSG msg);

    void sendScanCode(WORD hwScanCode, WORD vScanCode);

public:
    GlobalEventDispatcher(GlobalEventDispatcher const &) = delete;

    void operator=(GlobalEventDispatcher const &) = delete;

    bool init(HWND mainWindowHandle, QString &error);

    //bool eventFilter(QObject *watched, QEvent *event) override;

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE;

public slots:

    void handleKbdFnPress(const unsigned char fnKeyCode);

    void handleAcpiEvent(const unsigned long acpiCode);


    void releaseKey();
};


#endif //G14CONTROLPP_GLOBALEVENTDISPATCHER_H
