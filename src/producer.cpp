#include "producer.h"
#include "constants.h"
#include <chrono>
#include <thread>

void producer(Decoder* decoder, RingBuffer* buffer)
{
    {
        float bucket[constants::BUCKET_SIZE]; // A temporary buffer for chunk decoding

        // Streaming loop
        while (true)
        {
            drmp3_uint64 framesDecoded =  
                decoder->decodeMp3Chunk(
                    bucket, 
                    constants::BUCKET_SIZE / constants::CHANNELS
                );
            
            if (framesDecoded == 0) // end of file
                break;
            
            while (buffer->push(
                bucket, 
                framesDecoded * constants::CHANNELS
            )){
                // buffer is full (wait)
                 std::this_thread::sleep_for(
                    std::chrono::milliseconds(1)
                 );
            }
        }
    }
}