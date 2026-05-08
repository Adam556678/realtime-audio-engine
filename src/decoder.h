// A header file for audio files decoder
// author: Mahmoud Ramadan

#ifndef DECODER_H
#define DECODER_H

#include <iostream>
#include <string>
#include "../include/dr_mp3.h"
#include "../include/AudioData.h"

class Decoder{
    public:
        Decoder();

        bool openMp3(const char* path);
        bool openWAV(const char* path);

        float* decodeMp3(const char* filePath);
        float* decodeWAV(const char* filePath);
        drmp3_uint64 decodeMp3Chunk(float* output, size_t frames);
        drwav_uint64 decodeWAVChunk(float* output, size_t frames);
        
        void close();

        AudioData getAudioData();

    private:
        AudioData audioData;
        drmp3 mp3;
        drwav wav;
};

#endif