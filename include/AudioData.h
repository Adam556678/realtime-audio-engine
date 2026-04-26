#ifndef AUDIODATA_H
#define AUDIODATA_H

#include <cstdint>
struct AudioData
{

    AudioData() : channels(0), sampleRate(0), totalFrames(0) 
    {}

    AudioData(
        uint32_t channels, 
        uint32_t sampleRate, 
        uint64_t totalFrames
    ){
        this->channels = channels;
        this->sampleRate = sampleRate;
        this->totalFrames = totalFrames;
    }

    uint32_t channels;
    uint32_t sampleRate;
    uint64_t totalFrames;
};

#endif