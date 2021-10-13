#ifndef G14CONTROLPP_AUDIOUTILS_H
#define G14CONTROLPP_AUDIOUTILS_H

#include "windows.h"
#include <QString>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

template<class T>
void SafeRelease(T **ppT) {
    if (*ppT) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class AudioUtils {
public:

    static void toggleMute(WINBOOL &mute, QString &error);

};

#endif //G14CONTROLPP_AUDIOUTILS_H
