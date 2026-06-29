#include "consumer.h"

#include <thread>
#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <functiondiscoverykeys_devpkey.h>
#include <combaseapi.h>

void consume(RingBuffer* buffer){
    HRESULT hr = CoInitialize(nullptr);

    IMMDeviceEnumerator* enumerator = nullptr;

    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        nullptr,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void**)&enumerator
    );

    IMMDevice* device = nullptr;

    // Get the default output device
    hr = enumerator->GetDefaultAudioEndpoint(
        eRender, // Speakers/Headphones
        eConsole,
        &device
    );

    enumerator->Release();

    IAudioClient* audioClient = nullptr;
    hr = device->Activate(
        __uuidof(IAudioClient),
        CLSCTX_ALL,
        nullptr,
        (void**)&audioClient
    );

    device->Release();

}

void handleComException(HRESULT hr, std::string call){
    if (FAILED(hr)){
        std::cerr << "CoCreateInstance failed\n";
        return;
    }
}