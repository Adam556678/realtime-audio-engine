#ifndef CONSUMER_H
#define CONSUMER_H

#include "ring_buffer.h"
#include "include/PlaybackState.h"

void consume(RingBuffer* buffer, PlaybackState* state);

#endif
