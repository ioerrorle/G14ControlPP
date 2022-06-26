#include "AudioUtils.h"

void AudioUtils::toggleMute(WINBOOL &mute, QString &error) {
    UINT deviceCount = 0;
    IMMDeviceCollection *deviceCollection = NULL;
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    IAudioEndpointVolume *endpointVolumes[16];
    IMMDevice *devices[16];

    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {

        error = QString::asprintf("Unable to initialize COM: %lx\n", hr);
        goto Exit;
    }

    //We initialize the device enumerator here
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                          IID_PPV_ARGS(&deviceEnumerator));
    if (FAILED(hr)) {
        error = QString::asprintf("Unable to instantiate device enumerator: %lx\n", hr);
        goto Exit;
    }

    hr = deviceEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE,
                                              &deviceCollection);
    if (FAILED(hr)) {
        error = QString::asprintf("Unable to retrieve device collection: %lx\n", hr);
        goto Exit;
    }

    hr = deviceCollection->GetCount(&deviceCount);
    if (FAILED(hr)) {
        error = QString::asprintf("Unable to get device collection length: %lx\n", hr);
        goto Exit;
    }

    for (UINT i = 0; i < deviceCount; i++) {
        IMMDevice *device = NULL;
        hr = deviceCollection->Item(i, &device);
        devices[i] = device;
        if (FAILED(hr)) {
            endpointVolumes[i] = NULL;
            error = QString::asprintf("Unable to retrieve device %d: %lx\n", i, hr);
            continue;
        }

        IAudioEndpointVolume *endpointVolume = NULL;

        hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL,
                              reinterpret_cast<void **>(&endpointVolume));
        endpointVolumes[i] = endpointVolume;
        if (FAILED(hr)) {
            endpointVolumes[i] = NULL;
            error = QString::asprintf("Unable to activate endpoint volume on output device: %lx\n", hr);
            continue;
        }
    }

    for (UINT j = 0; j < deviceCount; j++) {
        IAudioEndpointVolume *endpointVolume = endpointVolumes[j];
        if (endpointVolume == NULL) {
            continue;
        }

        hr = endpointVolume->GetMute(&mute); //Try to mute endpoint here
        if (FAILED(hr)) {
            error = QString::asprintf("Unable to set mute state on endpoint: %lx\n", hr);
        } else {
            break;
        }
    }

    if (mute) {
        mute = FALSE;
    } else {
        mute = TRUE;
    }

    for (UINT j = 0; j < deviceCount; j++) {
        IAudioEndpointVolume *endpointVolume = endpointVolumes[j];
        if (endpointVolume == NULL) {
            continue;
        }

        hr = endpointVolume->SetMute(mute, NULL); //Try to mute endpoint here
        if (FAILED(hr)) {
            error = QString::asprintf("Unable to set mute state on endpoint: %lx\n", hr);
        }
    }
    Exit:
    for (UINT i = 0; i < deviceCount; i++) {
        SafeRelease(&devices[i]);
        SafeRelease(&endpointVolumes[i]);
    }
    SafeRelease(&deviceCollection);
    SafeRelease(&deviceEnumerator);
    CoUninitialize();
};
