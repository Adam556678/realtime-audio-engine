#ifndef PRODUCER_H
#define PRODUCER_H

#include "decoder.h"
#include "ring_buffer.h"
#include "include/PlaybackState.h"

void producer(
    Decoder* decoder, 
    RingBuffer* buffer,
    PlaybackState* state
);

#endif