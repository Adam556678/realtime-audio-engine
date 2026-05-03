// Circular Buffer data structure for storing PCM samples
// Author: Mahmoud Ramadan

#include "ring_buffer.h"

RingBuffer::RingBuffer(int size) : 
    size(size), lastInsert(-1), nextRead(0)
{
    data.resize(size);
}

bool RingBuffer::push(float sample)
{
    int nextInsert = (this->lastInsert + 1) % this->size;
    // Prevent overwriting unread samples
    if (nextInsert == nextRead){
        return false;
    }
    
    this->lastInsert = nextInsert;
    data[lastInsert] = sample;

    return true;
}

bool RingBuffer::pop()
{
    return false;
}

bool RingBuffer::empty() const
{
    return false;
}

bool RingBuffer::full() const
{
    return false;
}
