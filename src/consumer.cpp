#include "consumer.h"
#include "constants.h"

#include <thread>
#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <functiondiscoverykeys_devpkey.h>
#include <combaseapi.h>

struct WasapiContext {
    IAudioClient* audioClient;
    IAudioRenderClient* renderClient;
    UINT32 bufferFrames;
    HANDLE event;
};

void consume(RingBuffer* buffer){
    HRESULT hr;
   
    // Configure WASAPI
    WasapiContext context = WasapiContext();
    hr = config_wasapi(context);
    
    if (FAILED(hr))
    {
        std::cerr << "WASAPI configuration failed\n";
        return ;
    }

    // Start playback
    IAudioClient* audioClient = context.audioClient; 
    hr = audioClient->Start();
    
    if (FAILED(hr))
    {
        std::cerr << "audio client start failed\n";
        return ;
    }
    
    // Play....
    while (true)
    {

        // Pause this thread untill event fires
        WaitForSingleObject(
            context.event, // Event to wait for
            INFINITE // wait forever
        );

        // get current padding in the buffer
        UINT32 padding;
        hr = audioClient->GetCurrentPadding(&padding);

        if (FAILED(hr))
        {
            std::cerr << "Getting buffer's padding failed\n";
            return ;
        }
        
        // Get available size in output's buffer
        UINT32 available = context.bufferFrames - padding;
        if (available == 0){
            // wait for the buffer to free space
            Sleep(1); 
            continue;
        }
        
        // get access to the speaker/headphone buffer
        IAudioRenderClient* renderClient = context.renderClient;
        BYTE* data = nullptr;
        hr = renderClient->GetBuffer(available, &data);
        
        if (FAILED(hr))
        {
            std::cerr << "Accessing to output's buffer failed\n";
            return ;
        }

        // pop from the engine buffer to the window's buffer
        buffer->pop(
            reinterpret_cast<float*>(data), 
            available * constants::CHANNELS
        );

        // Finish writing to the buffer
        renderClient->ReleaseBuffer(available, 0);
    }
    
    
}

HRESULT config_wasapi(WasapiContext &context){
     HRESULT hr = CoInitialize(nullptr);

    if (FAILED(hr)){
        std::cerr <<"CoInitialize failed\n";
        return hr;
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
        return hr;
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
        return hr;
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
        return hr;
    }

    device->Release();
    
    // hnsBufferDuration = 1 second; // Create a buffer that can hold 1 second of sound.

    // get device's preferred format
    WAVEFORMATEX* format = nullptr;
    audioClient->GetMixFormat(&format);

    // Create windows event object
    HANDLE event = CreateEvent(
        nullptr, // who is allowed to access the event (default security)
        FALSE, // Auto reset event
        FALSE,
        nullptr // Event name
    );

    hr = audioClient->Initialize(
        AUDCLNT_SHAREMODE_SHARED, // mix all sounds together
        AUDCLNT_STREAMFLAGS_EVENTCALLBACK, //Notify me whenever you need more audio
        200000, // the amount of time the buffer should be able to hold (20ms).
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
        return hr;
    }
    
    // Set event handle
    hr = audioClient->SetEventHandle(event);

    if (FAILED(hr))
    {
        std::cerr << "Setting event handle failed\n";
        CoTaskMemFree(format);
        audioClient->Release();
        CoUninitialize();
        return hr;
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
        return hr;
    }

    UINT32 bufferFrames = 0;
    hr = audioClient->GetBufferSize(&bufferFrames);

    if (FAILED(hr))
    {
        std::cerr << "GetBufferSize failed\n";
        return hr;
    }

    // Assign values to context
    context.audioClient = audioClient;
    context.bufferFrames = bufferFrames;
    context.renderClient = renderClient;
    context.event = event;

    return hr;
}
