#include "consumer.h"
#include "constants.h"
#include "../include/PlaybackState.h"
#include "../include/AudioData.h"

#include <algorithm>
#include <thread>
#include <iostream>
#define NOMINMAX
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


HRESULT config_wasapi(WasapiContext &context, AudioData audioData){
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
    // WAVEFORMATEX* format = nullptr;
    // audioClient->GetMixFormat(&format);

    WAVEFORMATEX fileFormat {};

    fileFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
    fileFormat.nChannels = audioData.channels;
    fileFormat.nSamplesPerSec = audioData.sampleRate;
    fileFormat.wBitsPerSample = 32;
    fileFormat.nBlockAlign =
        fileFormat.nChannels * sizeof(float);
    fileFormat.nAvgBytesPerSec =
        fileFormat.nSamplesPerSec * fileFormat.nBlockAlign;
    fileFormat.cbSize = 0;

    // Create windows event object
    HANDLE event = CreateEvent(
        nullptr, // who is allowed to access the event (default security)
        FALSE, // Auto reset event
        FALSE,
        nullptr // Event name
    );

    DWORD flags = AUDCLNT_STREAMFLAGS_EVENTCALLBACK | //Notify me whenever you need more audio
    AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM |
    AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY; // Window's sample rate conversion

    hr = audioClient->Initialize(
        AUDCLNT_SHAREMODE_SHARED, // mix all sounds together
        flags, 
        200000, // the amount of time the buffer should be able to hold (20ms).
        0, // Only important in Exclusive Mode
        &fileFormat,
        nullptr
    );

    if (FAILED(hr))
    {
        std::cerr << "Initialize failed\n";
        audioClient->Release();
        CoUninitialize();
        return hr;
    }
    
    // Set event handle
    hr = audioClient->SetEventHandle(event);

    if (FAILED(hr))
    {
        std::cerr << "Setting event handle failed\n";
        audioClient->Release();
        CoUninitialize();
        return hr;
    }

    // We don't need the format structure anymore.

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


void consume(RingBuffer* buffer, PlaybackState* state, AudioData audioData){
    HRESULT hr;
   
    ///// DEBUG
    std::cout << "Start consuming...\n";

    // Configure WASAPI
    WasapiContext context = WasapiContext();
    hr = config_wasapi(context, audioData);
    
    if (FAILED(hr))
    {
        std::cerr << "WASAPI configuration failed\n";
        state->stopRequested = true;
        return ;
    }

    // Start playback
    IAudioClient* audioClient = context.audioClient; 

    // get access to the speaker/headphone buffer
    IAudioRenderClient* renderClient = context.renderClient;

    // --------------- Wait for initial Data ----------------// 
    
    size_t initialSamples = context.bufferFrames * audioData.channels;
    
    while (buffer->getAvailableSamples() < initialSamples &&
    !state->finished)
    {
        std::cout << "---> CONSUMER: " << "Not enough samples in buffer, waiting for samples..."<< std::endl;
        
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1)
        );
    }
    
    // --------------- Prefill WASAPI buffer ----------------// 
    
    BYTE* data = nullptr;
    hr = renderClient->GetBuffer(
        context.bufferFrames,
        &data
    );
    
    if (FAILED(hr))
    {
        std::cerr << "Initial GetBuffer failed\n";
        state->stopRequested = true;
        return;
    }
    
    ////// DEBUG
    std::cout << "--> CONSUMER : Before prefilling WASAPI buffer" << std::endl;
    
    if (!buffer->pop(
        reinterpret_cast<float*>(data),
        initialSamples
    )){
        std::cerr << "--> CONSUMER : prefilling WASAPI buffer failed.\n";
        return ;
    }

    ////// DEBUG
    std::cout << "--> CONSUMER : Initial samples filled" << std::endl;
    
    hr = renderClient->ReleaseBuffer(
        context.bufferFrames,
        0
    );
    
    if (FAILED(hr))
    {
        std::cerr << "Initial ReleaseBuffer failed\n";
        state->stopRequested = true;
        return;
    }
    
    // --------------- Start Playback ----------------// 
    
    hr = audioClient->Start();
    
    if (FAILED(hr))
    {
        std::cerr << "audio client start failed\n";
        state->stopRequested = true;
        return ;
    }
    
    // Play....
    while (!state->finished || !buffer->empty())
    {
        
        // Pause this thread untill event fires
        // Wake this thread when there's free space in the buffer
        WaitForSingleObject(
            context.event, // Event to wait for
            INFINITE // wait forever
        );
        
        ////// DEBUG
        std::cout << "--> CONSUMER : Playing......" << std::endl;

        // get current padding in the buffer
        UINT32 padding;
        hr = audioClient->GetCurrentPadding(&padding);

        if (FAILED(hr))
        {
            std::cerr << "Getting buffer's padding failed\n";
            state->stopRequested = true;
            return ;
        }
        
        // Get available size in output's buffer
        UINT32 available = context.bufferFrames - padding;
        if (available == 0){
            continue;
        }

        BYTE* data = nullptr;
        hr = renderClient->GetBuffer(available, &data);
        
        if (FAILED(hr))
        {
            std::cerr << "Accessing to output's buffer failed\n";
            state->stopRequested = true;
            return ;
        }

        // get the number of samples needed
        size_t samplesNeeded = 
            available * audioData.channels;
        
        // get the number of available samples in the ring buffer
        size_t availableSamples = 
            buffer->getAvailableSamples();

        size_t samplesToCopy = 
            std::min(availableSamples, samplesNeeded);

        // pop from the engine buffer to the window's buffer
        if (samplesToCopy > 0)
        {
            buffer->pop(
                reinterpret_cast<float*>(data),
                samplesToCopy
            );
        }

        // Fill the rest with silence
        if (samplesToCopy < samplesNeeded)
        {
            std::memset(
                reinterpret_cast<float*>(data) + samplesToCopy,
                0,
                (samplesNeeded - samplesToCopy) * sizeof(float)
            );
        }

        // Commit the complete WASAPI buffer
        renderClient->ReleaseBuffer(available, 0);

    }
    
}
