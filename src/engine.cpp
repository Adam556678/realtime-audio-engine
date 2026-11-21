// Author: Mahmoud Ramadan
// The public interface of the audio engine

#include "../include/engine.h"
#include "../include/file_dialog.h"
#include "../include/extensions.h"
#include "../include/helpers.h"
#include "producer.h"
#include "consumer.h"
#include "constants.h"

#include <thread>

Engine::Engine()
    :buffer(constants::BUFFER_FRAMES * constants::CHANNELS){

}

bool Engine::pickSong(){
    // Open file dialog to pick an audio file
    std::string filePath = openFileDialog();

    if (filePath.empty()) {
        return false;
    }

    // Capture file's name & extension
    Extension ext = Helpers::getExtension(filePath);
    std::string title = Helpers::getSongTitle(filePath);

    // Open audio file
    if (ext == MP3){
        if (!decoder.openMp3(filePath.c_str())){
            std::cerr << "Failed to open file";
            return false;
        }
    } else if (ext == WAV){
        if (!decoder.openWAV(filePath.c_str())){
            std::cerr << "Failed to open file";
            return false;
        }
    }else
    {
        std::cerr << "Uknown file extension";
        return false;
    }

    playbackState.audioExtension = ext;
    playbackState.songTitle = title;
}

void Engine::process(){
    // std::thread producerThread(
    //     producer,
    //     &decoder,
    //     &buffer,
    //     &playbackState
    // );
}

void Engine::play(){
    std::thread producerThread(
        producer,
        &decoder,
        &buffer,
        &playbackState
    );

    std::thread consumerThread(
        consume,
        &buffer,
        &playbackState,
        decoder.getAudioData()
    );
}

void Engine::pause(){}

void Engine::resume(){}

void Engine::stop(){}