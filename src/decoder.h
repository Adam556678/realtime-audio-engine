// A header file for audio files decoder
// author: Mahmoud Ramadan

#ifndef DECODER_H
#define DECODER_H

#include <iostream>
#include <string>
#include "dr_mp3.h"
#include <AudioData.h>

class Decoder{
    public:
        Decoder();
        float* decodeMp3(const char* filePath);
        float* decodeWAV(const char* filePath);

        AudioData getAudioData();

    private:
        AudioData audioData;
};

#endif