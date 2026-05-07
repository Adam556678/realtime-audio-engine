#include "include/dr_mp3.h"
#include "src/decoder.h"
#include "src/constants.h"
#include "src/ring_buffer.h"
#include "src/producer.h"
#include <string>
#include <thread>

int main(){

    Decoder decoder;
    std::string filePath = "F:/coding/Low Level System Engineering/--------- Final Projects ---------/Audio Engine/realtime-audio-engine/audio.mp3";

    RingBuffer buffer(constants::BUFFER_FRAMES * constants::CHANNELS); // Circular buffer
    
    // Open audio file
    decoder.openMp3(filePath.c_str());

    std::thread producerThread(
        producer,
        &decoder,
        &buffer
    );

    producerThread.join();
    
    decoder.close();

    return 0;
}
