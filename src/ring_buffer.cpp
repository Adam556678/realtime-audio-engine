// Circular Buffer data structure for storing PCM samples
// Author: Mahmoud Ramadan

#include <iostream>
#include "ring_buffer.h"

RingBuffer::RingBuffer(int size) : 
    size(size), insertPos(0), readPos(0), count(0)
{
    data.resize(size);
}

bool RingBuffer::push(const float* input, size_t samples)
{
    // Prevent overwriting unread samples
    if (samples > (size - count))
        return false;

    // DEBUG
    std::cout << "---> BUFFER: " << samples << " samples are pushed"<< std::endl;
    
    for (size_t i = 0; i < samples; i++)
    {
        data[(i+insertPos) % size] = input[i];
    }
    
    insertPos = (insertPos + samples) % size;
    
    count += samples;

    return true;
}


bool RingBuffer::pop(float* output, size_t samples)
{
    if (empty() || samples > count)
        return false;
    
    for (size_t i = 0; i < samples; i++)
    {
        output[i] = data[(readPos+i) % size];
    }
    
    readPos = (readPos + samples) % size;
    count -= samples;

    return true;
}

bool RingBuffer::empty() const
{
    return count == 0;
}

bool RingBuffer::full() const
{
    return count == size;
}
