#include <conio.h>
#include <iostream>
#include <windows.h>
#include "../include/PlaybackState.h"

void handleKeyboard(PlaybackState* state)
{
    while (!state->finished && !state->stopRequested)
    {
        if (_kbhit())
        {
            char key = _getch();

            if (key == ' ')
            {
                std::cout << "[ACTION] Spacebar detected!\n";

                // Toggle pause state
                state->paused = !state->paused;

                // Notify the consumer that playback state changed
                SetEvent(state->controlEvent);
            }
        }

        Sleep(10);
    }
}
