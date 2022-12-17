#pragma once
#ifndef YAYA_AUDIO_H
#define YAYA_AUDIO_H

#include <SFML/Audio.hpp>
#include <string>

class Audio {
private :
    inline static sf::SoundBuffer buffer;
    inline static sf::Sound sound;
public :
    inline Audio() = default;

    inline Audio(sf::SoundBuffer b, sf::Sound s){
        this->buffer = b;
        this->sound = s;
    }

    inline ~Audio() = default;

    static void playSound(std::string fileName);
};

#endif