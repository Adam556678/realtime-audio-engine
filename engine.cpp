#include "src/decoder.h"
#include "src/constants.h"
#include "include/PlaybackState.h"
#include "src/keyboard_inp_controller.h"
#include "src/ring_buffer.h"
#include "src/producer.h"
#include "src/consumer.h"
#include "include/file_dialog.h"
#include "include/extensions.h"
#include "include/helpers.h"
#include <string>
#include <thread>
#include <iostream>

int main(){

    Decoder decoder;

    // Open file dialog to pick an audio file
    std::string filePath = openFileDialog();
    
    if (filePath.empty()) {
        std::cout << "No file was selected.\n";
        return 1;
    }

    // Capture file's extension
    Extension ext = Helpers::getExtension(filePath);

    RingBuffer buffer(constants::BUFFER_FRAMES * constants::CHANNELS); // Circular buffer
    
    // Open audio file
    if (ext == MP3){
        if (!decoder.openMp3(filePath.c_str())){
            std::cerr << "Failed to open file";
            return 1;
        }
    } else if (ext == WAV){
        if (!decoder.openWAV(filePath.c_str())){
            std::cerr << "Failed to open file";
            return 1;
        }
    }else
    {
        std::cerr << "Uknown file extension";
        return 1;
    }
    
    

    // Create a shared playback state between producer & consumer
    PlaybackState state;
    state.audioExtension = ext;

    state.controlEvent = CreateEvent(
        nullptr,
        FALSE,
        FALSE,
        nullptr
    );

    if (state.controlEvent == nullptr)
    {
        std::cerr << "Failed to create control event\n";
        return 1;
    }

    std::thread producerThread(
        producer,
        &decoder,
        &buffer,
        &state
    );

    std::thread consumerThread(
        consume,
        &buffer,
        &state,
        decoder.getAudioData()
    );

    std::thread keyboardThread(
        handleKeyboard,
        &state
    );

    producerThread.join();
    consumerThread.join();
    keyboardThread.join();
    
    decoder.close();

    return 0;
}
