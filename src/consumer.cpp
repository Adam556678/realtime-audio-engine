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

    if (FAILED(hr)){
        std::cerr <<"CoInitialize failed\n";
        return;
    }
    
    IMMDeviceEnumerator* enumerator = nullptr;
    
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        nullptr,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void**)&enumerator
    );

    if (FAILED(hr)){
        std::cerr <<"CoCreateInstance failed\n";
        CoUninitialize();
        return;
    }

    IMMDevice* device = nullptr;

    // Get the default output device
    hr = enumerator->GetDefaultAudioEndpoint(
        eRender, // Speakers/Headphones
        eConsole,
        &device
    );

    if (FAILED(hr)){
        std::cerr <<"Enumerator failed\n";
        return;
    }

    enumerator->Release();

    IAudioClient* audioClient = nullptr;
    hr = device->Activate(
        __uuidof(IAudioClient),
        CLSCTX_ALL,
        nullptr,
        (void**)&audioClient
    );

    if (FAILED(hr)){
        std::cerr <<"Activate failed\n";
        return;
    }

    device->Release();
    
    // hnsBufferDuration = 1 second; // Create a buffer that can hold 1 second of sound.

    // get device's preferred format
    WAVEFORMATEX* format = nullptr;
    audioClient->GetMixFormat(&format);

    hr = audioClient->Initialize(
        AUDCLNT_SHAREMODE_SHARED, // mix all sounds together
        0, //No flags
        200000, // the amount of time the buffer should be able to hold.
        0, // Only important in Exclusive Mode
        format,
        nullptr
    );

    if (FAILED(hr))
    {
        std::cerr << "Initialize failed\n";
        CoTaskMemFree(format);
        audioClient->Release();
        CoUninitialize();
        return;
    }

    // We don't need the format structure anymore.
    CoTaskMemFree(format);

    IAudioRenderClient* renderClient = nullptr;

    hr = audioClient->GetService(
        __uuidof(IAudioRenderClient),
        (void**)&renderClient
    );

    if (FAILED(hr))
    {
        std::cerr << "GetService failed\n";
        audioClient->Release();
        CoUninitialize();
        return;
    }

    UINT32 bufferFrames = 0;
    hr = audioClient->GetBufferSize(&bufferFrames);

    if (FAILED(hr))
    {
        std::cerr << "GetBufferSize failed\n";
    }

    
}