
// Decoder for converting audio files (MP3 & WAV) to PCM samples
// Author: Mahmoud Ramadan

#define DR_MP3_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION

#include "decoder.h"
#include "../include/dr_mp3.h"
#include "../include/dr_wav.h"

Decoder::Decoder()
{
}

float *Decoder::decodeMp3(const char* filePath)
{

    drmp3_config config;
    drmp3_uint64 totalFrameCount;

    float* pcm = drmp3_open_file_and_read_pcm_frames_f32(
        filePath,
        &config,
        &totalFrameCount,
        NULL
    );

    if (!pcm){
        throw std::runtime_error("Failed to decode MP3 file.");
    }

    this->audioData = AudioData(config.channels, config.sampleRate, totalFrameCount);

    return pcm;
}

float *Decoder::decodeWAV(const char* filePath)
{
    drwav wav;

    if (!drwav_init_file(&wav, filePath, NULL)){
        throw std::runtime_error("Failed to decode WAV file.");
    }

    // Assign metadata
    this->audioData = AudioData(
        wav.channels,
        wav.sampleRate,
        wav.totalPCMFrameCount
    );

    // Allocate PCM buffer
    float* pcm = new float[wav.totalPCMFrameCount * wav.channels];

    // write decoded WAV file (PCM samples) into the buffer
    drwav_read_pcm_frames_f32(
        &wav,
        wav.totalPCMFrameCount,
        pcm
    );

    drwav_uninit(&wav);

    return pcm;
}

drmp3_uint64 Decoder::decodeMp3Chunk(float* output, size_t frames)
{
    return drmp3_read_pcm_frames_f32(
        &mp3,
        frames,
        output
    );
}

drwav_uint64 Decoder::decodeWAVChunk(float* output, size_t frames)
{
    return drwav_read_pcm_frames_f32(
        &wav,
        frames,
        output
    );
}


AudioData Decoder::getAudioData()
{
    return this->audioData;
}

bool Decoder::openMp3(const char *path)
{
    if (!drmp3_init_file(&mp3, path, NULL))
        return false;
    
    return true;
}

bool Decoder::openWAV(const char *path)
{
    if (!drwav_init_file(&wav, path, NULL))
        return false;

    return true;
}

void Decoder::close()
{
    drmp3_uninit(&mp3);
}
