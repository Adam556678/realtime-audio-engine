#include "include/dr_mp3.h"
#include "src/decoder.h"
#include "src/constants.h"
#include "src/ring_buffer.h"
#include <string>

int main(){

    Decoder decoder;
    std::string filePath = "F:/coding/Low Level System Engineering/--------- Final Projects ---------/Audio Engine/realtime-audio-engine/audio.mp3";

    // // Decode MP3 audio file
    // float* pcm = decoder.decodeMp3(filePath.c_str());

    // // Get decoder's metadata
    // AudioData metadata = decoder.getAudioData();

    // // Display PCM samples
    // for (int i = 45870; i < metadata.totalFrames * metadata.channels; i++)
    // {
    //     if (pcm[i] != 0){
    //         std::cout << "index: " << i << " " << pcm[i] << "\n";
    //     }
    // }

    RingBuffer buffer(constants::BUFFER_FRAMES * constants::CHANNELS); // Circular buffer
    float bucket[constants::BUCKET_SIZE]; // A temporary buffer for chunk decoding

    // Streaming loop
    while (true)
    {
        drmp3_uint64 framesDecoded =  
            decoder.decodeMp3Chunk(
                bucket, 
                constants::BUCKET_SIZE / constants::CHANNELS
            );
        
        if (framesDecoded == 0) // end of file
            break;
        
        bool isDataPushed =  buffer.push(
            bucket, 
            framesDecoded * constants::CHANNELS
        );

        if (!isDataPushed){
            // Wait
        }
    }
    
    decoder.close();

    return 0;
}
