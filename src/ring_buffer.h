// Circular Buffer data structure for storing PCM samples
// Author: Mahmoud Ramadan

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <vector>
#include <ctime>

class RingBuffer
{
    private:    
        std::vector<float> data;
        
        size_t size;
        size_t insertPos;
        size_t readPos;
        size_t count;

    public:    
        RingBuffer(int size);

        bool push(const float* input, size_t samples);
        bool pop(float* output, size_t samples);

        bool empty() const;
        bool full() const;
        
};    


#endif