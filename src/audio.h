#pragma once
#ifndef YAYA_AUDIO_H
#define YAYA_AUDIO_H

#include <SFML/Audio.hpp>
#include <string>
#include <vector>

class Audio {
private :
    inline static int samples;
    inline static sf::SoundBuffer buffer;
    inline static std::vector<sf::SoundBuffer> bufferVec;
    inline static sf::Sound sound;
public :
    inline Audio() = default;

    inline ~Audio() = default;

    static void loadSound(std::string fileName);

    static void playSound(int id);
};

#endif