#ifndef CONSUMER_H
#define CONSUMER_H

#include "ring_buffer.h"
#include "../include/PlaybackState.h"
#include "../include/AudioData.h"


void consume(RingBuffer* buffer, PlaybackState* state, AudioData audioData);

#endif
