//
// Created by ioerror on 23.09.2021.
//

#ifndef G14CONTROLPP_KBDLISTENERSINGLETON_H
#define G14CONTROLPP_KBDLISTENERSINGLETON_H

#include "ListenerThread.h"

class KbdListenerSingleton {
public:
    static KbdListenerSingleton &getInstance();

private:
    KbdListenerSingleton();
    ListenerThread *workerThread;

public:
    KbdListenerSingleton(KbdListenerSingleton const &) = delete;

    void operator=(KbdListenerSingleton const &) = delete;

    bool init();

    void startListening();
};


#endif //G14CONTROLPP_KBDLISTENERSINGLETON_H
