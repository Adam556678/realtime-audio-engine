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
        
        int size;
        int lastInsert;
        int nextRead;

    public:    
        RingBuffer(int size);

        bool push(float sample);
        bool pop();

        bool empty() const;
        bool full() const;
        
};    


#endif