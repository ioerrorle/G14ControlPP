#ifndef G14CONTROLPP_GLOBALEVENTDISPATCHER_H
#define G14CONTROLPP_GLOBALEVENTDISPATCHER_H

#include <QAbstractNativeEventFilter>
#include <QAbstractEventDispatcher>
#include <QDebug>
#include <windows.h>
#include <QObject>
#include <QtCore/QAbstractEventDispatcher>
#include "KbdListenerThread.h"
#include "AcpiListenerThread.h"

class GlobalEventDispatcher : public QObject, public QAbstractNativeEventFilter {
Q_OBJECT

public:
    static GlobalEventDispatcher &getInstance();

private:
    GlobalEventDispatcher();

    KbdListenerThread *kbdListenerThread;
    AcpiListenerThread *acpiListenerThread;

    void handlePowerCfgChange(MSG msg);

public:
    GlobalEventDispatcher(GlobalEventDispatcher const &) = delete;

    void operator=(GlobalEventDispatcher const &) = delete;

    bool init(QString &error);

    //bool eventFilter(QObject *watched, QEvent *event) override;

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE;

public slots:

    void handleKbdFnPress(const unsigned char fnKeyCode);

    void handleKbdThreadFinished(QPrivateSignal signal);

    void handleAcpiEvent(const unsigned long acpiCode);

    void handleAcpiThreadFinished(QPrivateSignal signal);


};


#endif //G14CONTROLPP_GLOBALEVENTDISPATCHER_H
