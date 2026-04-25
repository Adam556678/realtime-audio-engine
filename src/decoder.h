// A header file for audio files decoder
// author: Mahmoud Ramadan

#ifndef DECODER_H
#define DECODER_H

#include <iostream>
#include <string>
#include "dr_mp3.h"

class Decoder{
    public:
        Decoder();
        float* decodeMp3(std::string filePath);
        float* decodeWAV(std::string filePath);

        drmp3_uint32 getChannles();
        drmp3_uint32 getSampleRate();
        drmp3_uint32 getTotalFrameCount();

    private:
        drmp3_config config;
        drmp3_uint64 totalFrameCount;
};

#endif