#include "include/dr_mp3.h"
#include "src/decoder.h"
#include <string>

int main(){

    Decoder decoder;
    std::string filePath = "F:/coding/Low Level System Engineering/--------- Final Projects ---------/Audio Engine/realtime-audio-engine/audio.mp3";

    // Decode MP3 audio file
    float* pcm = decoder.decodeMp3(filePath.c_str());

    // Get decoder's metadata
    AudioData metadata = decoder.getAudioData();

    // Display PCM samples
    for (int i = 45870; i < metadata.totalFrames * metadata.channels; i++)
    {
        if (pcm[i] != 0){
            std::cout << "index: " << i << " " << pcm[i] << "\n";
        }
    }
    
    drmp3_free(pcm, NULL);

    return 0;
}
