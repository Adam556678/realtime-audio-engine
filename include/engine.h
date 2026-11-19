// Author: Mahmoud Ramadan
// The public interface of the audio engine

#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include "../src/decoder.h"
#include "../src/ring_buffer.h"
#include "PlaybackState.h"

class Engine
{
private:
    std::string filepath;
    Decoder decoder;
    RingBuffer buffer;
    PlaybackState playbackState;
public:
    Engine();

    bool pickSong();
    void process();
    void play();
    void resume();
    void pause();
    void stop();
};

#endif