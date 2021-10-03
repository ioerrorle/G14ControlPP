#include "GlobalEventDispatcher.h"

GlobalEventDispatcher &GlobalEventDispatcher::getInstance() {
    static GlobalEventDispatcher instance;
    return instance;
}

GlobalEventDispatcher::GlobalEventDispatcher() {
}

bool GlobalEventDispatcher::nativeEventFilter(const QByteArray &eventType, void *message, long *) {
    MSG *msg = static_cast< MSG * >( message );

    if (msg->message == WM_POWERBROADCAST) {
        handlePowerCfgChange(*msg);
        /*qDebug() << ("Power broadcast");
        switch (msg->wParam) {
            case PBT_APMPOWERSTATUSCHANGE:
                qDebug() << ("PBT_APMPOWERSTATUSCHANGE  received\n");
                break;
            case PBT_APMRESUMEAUTOMATIC:
                qDebug() << ("PBT_APMRESUMEAUTOMATIC  received\n");
                break;
            case PBT_APMRESUMESUSPEND:
                qDebug() << ("PBT_APMRESUMESUSPEND  received\n");
                break;
            case PBT_APMSUSPEND:
                qDebug() << ("PBT_APMSUSPEND  received\n");
                break;
        }*/

    }
    return false;
}

bool GlobalEventDispatcher::init(QString &error) {
    QString err;
    //subscribe to global events
    QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    //create kbd listener thread and connect to it

    this->kbdListenerThread = new KbdListenerThread(err);
    connect(this->kbdListenerThread, &KbdListenerThread::resultReady, this, &GlobalEventDispatcher::handleKbdFnPress);
    connect(this->kbdListenerThread, &KbdListenerThread::finished, this,
            &GlobalEventDispatcher::handleKbdThreadFinished);

    this->acpiListenerThread = new AcpiListenerThread(err);
    connect(this->acpiListenerThread, &AcpiListenerThread::resultReady, this, &GlobalEventDispatcher::handleAcpiEvent);
    connect(this->acpiListenerThread, &AcpiListenerThread::finished, this,
            &GlobalEventDispatcher::handleAcpiThreadFinished);

}

void GlobalEventDispatcher::handleKbdFnPress(const unsigned char fnKeyCode) {

}

void GlobalEventDispatcher::handleAcpiEvent(const unsigned long acpiCode) {

}

void GlobalEventDispatcher::handlePowerCfgChange(MSG msg) {

}

void GlobalEventDispatcher::handleKbdThreadFinished(GlobalEventDispatcher::QPrivateSignal signal) {

}

void GlobalEventDispatcher::handleAcpiThreadFinished(GlobalEventDispatcher::QPrivateSignal signal) {

}


