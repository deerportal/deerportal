#ifndef SOUNDFX_H
#define SOUNDFX_H
#include <iostream>

#include <SFML/Audio.hpp>

class SoundFX
{
public:
    SoundFX();

    sf::SoundBuffer soundCollectBuffer;
    sf::Sound soundCollect;
    void playCollect();

    sf::SoundBuffer soundLetsBeginBuffer;
    sf::Sound soundLetsBegin;
    void playLetsBegin();
};

#endif // SOUNDFX_H
