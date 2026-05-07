#ifndef PRODUCER_H
#define PRODUCER_H

#include "decoder.h"
#include "ring_buffer.h"

void producer(Decoder* decoder, RingBuffer* buffer);

#endif