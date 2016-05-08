#include "soundfx.h"

SoundFX::SoundFX()
{
    if (!soundCollectBuffer.loadFromFile("assets/audio/collect.ogg"))
        exit(-1);
    if (!soundCardBuffer.loadFromFile("assets/audio/card.ogg"))
        exit(-1);
    soundCollect.setBuffer(soundCollectBuffer);
    soundCollect.setVolume(20);

    soundCard.setBuffer(soundCardBuffer);
//    soundCollect.setVolume(20);

    if (!soundLetsBeginBuffer.loadFromFile("assets/audio/letsbegin.ogg"))
        exit(-1);
    soundLetsBegin.setBuffer(soundLetsBeginBuffer);
    soundLetsBegin.setVolume(20);
}

void SoundFX::playCollect()
{
    soundCollect.play();
}

void SoundFX::playCard()
{
    soundCard.play();
}

void SoundFX::playLetsBegin()
{
//    std::cout << "playing.... begin lets" << std::endl;
    soundLetsBegin.play();
}
