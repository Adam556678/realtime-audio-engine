#ifndef PLAYBACKSTATE_H
#define PLAYBACKSTATE_H

#include<atomic>

struct PlaybackState
{
    std::atomic<bool> playing{true};
    std::atomic<bool> finished{false};
    std::atomic<bool> paused{false};
};

#endif