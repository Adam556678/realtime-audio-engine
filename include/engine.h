// Author: Mahmoud Ramadan
// The public interface of the audio engine

#ifndef ENGINE_H
#define ENGINE_H

#include <string>

class Engine
{
private:
    std::string filepath;
public:
    Engine();

    void pickSong(std::string path);
    void play();
    void resume();
    void pause();
    void stop();
};

#endif