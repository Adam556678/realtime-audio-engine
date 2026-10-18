#ifndef PLAYBACKSTATE_H
#define PLAYBACKSTATE_H

#include<atomic>
#include <windows.h>
#include "extensions.h"

struct PlaybackState
{
    std::atomic<bool> playing{true};
    std::atomic<bool> finished{false}; // Decoding finished
    std::atomic<bool> paused{false};
    std::atomic<bool> stopRequested{false}; // Consumer stopped requesting samples
    Extension audioExtension;
    HANDLE controlEvent = nullptr;
};

#endif