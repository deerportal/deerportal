#ifndef SOUNDFX_H
#define SOUNDFX_H

#include <SFML/Audio.hpp>

class SoundFX
{
public:
    SoundFX();

    sf::SoundBuffer soundCollectBuffer;
    sf::Sound soundCollect;
    void playCollect();
};

#endif // SOUNDFX_H
