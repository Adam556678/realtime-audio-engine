
// Decoder for converting audio files (MP3 & WAV) to PCM samples
// Author: Mahmoud Ramadan

#include "decoder.h"
#include <dr_wav.h>

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

AudioData Decoder::getAudioData()
{
    return this->audioData;
}

