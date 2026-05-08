#include "producer.h"
#include "constants.h"
#include <chrono>
#include <thread>
#include <iostream>

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

            // DEBUG
            std::cout << "---> BUCKET: " << framesDecoded * constants::CHANNELS << " samples filled" << std::endl;
            
            if (framesDecoded == 0) // end of file
                break;
            
            while (!buffer->push(
                bucket, 
                framesDecoded * constants::CHANNELS
            )){
                // buffer is full (wait)

                // DEBUG
                std::cout << "---> BUFFER: " << "Buffer is full, waiting for consuming..."<< std::endl;

                 std::this_thread::sleep_for(
                    std::chrono::milliseconds(1)
                 );
            }
        }
    }
}