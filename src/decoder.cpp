#include "decoder.h"

Decoder::Decoder()
{
}

float *Decoder::decodeMp3(std::string filePath)
{
    return nullptr;
}

float *Decoder::decodeWAV(std::string filePath)
{
    return nullptr;
}

drmp3_uint32 Decoder::getChannles()
{
    return drmp3_uint32();
}

drmp3_uint32 Decoder::getSampleRate()
{
    return drmp3_uint32();
}

drmp3_uint32 Decoder::getTotalFrameCount()
{
    return drmp3_uint32();
}
