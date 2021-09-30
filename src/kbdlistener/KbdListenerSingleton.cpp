#include "KbdListenerSingleton.h"

KbdListenerSingleton &KbdListenerSingleton::getInstance() {
    static KbdListenerSingleton instance;
    return instance;
}

KbdListenerSingleton::KbdListenerSingleton() {

}

bool KbdListenerSingleton::init() {
    QString error;
    this->workerThread = new KbdListenerThread(error);
    return true;
}

void KbdListenerSingleton::startListening() {
    workerThread->start();
}
