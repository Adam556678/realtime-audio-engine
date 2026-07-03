#include "include/dr_mp3.h"
#include "src/decoder.h"
#include "src/constants.h"
#include "include/PlaybackState.h"
#include "src/ring_buffer.h"
#include "src/producer.h"
#include "src/consumer.h"
#include <string>
#include <thread>

int main(){

    Decoder decoder;
    std::string filePath = "F:/coding/Low Level System Engineering/--------- Final Projects ---------/Audio Engine/realtime-audio-engine/audio.mp3";

    RingBuffer buffer(constants::BUFFER_FRAMES * constants::CHANNELS); // Circular buffer
    
    // Open audio file
    decoder.openMp3(filePath.c_str());

    // Create a shared playback state between producer & consumer
    PlaybackState state;

    std::thread producerThread(
        producer,
        &decoder,
        &buffer,
        &state
    );

    std::thread consumerThread(
        consume,
        &buffer,
        &state
    );

    producerThread.join();
    consumerThread.join();
    
    decoder.close();

    return 0;
}
