// A struct for audio metadata
// Author: Mahmoud Ramadan

#ifndef AUDIODATA_H
#define AUDIODATA_H

#include <cstdint>
#include "extensions.h"
struct AudioData
{

    AudioData() : channels(0), sampleRate(0), totalFrames(0), audioExtension(UNKNOWN)
    {}

    AudioData(
        uint32_t channels, 
        uint32_t sampleRate, 
        uint64_t totalFrames,
        Extension audioExtension
    ){
        this->channels = channels;
        this->sampleRate = sampleRate;
        this->totalFrames = totalFrames;
        this->audioExtension = audioExtension;
    }

    uint32_t channels;
    uint32_t sampleRate;
    uint64_t totalFrames; // A frame is one sample for EACH channel at a single moment in time
    Extension audioExtension;
};

#endif