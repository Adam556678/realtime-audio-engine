#include "decoder.h"

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

float *Decoder::decodeWAV(const std::string filePath)
{
    return nullptr;
}

AudioData Decoder::getAudioData()
{
    return this->audioData;
}

