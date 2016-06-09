#include "soundfx.h"

SoundFX::SoundFX()
{
    if (!soundCollectBuffer.loadFromFile("assets/audio/collect.ogg"))
        exit(-1);
    if (!soundCardBuffer.loadFromFile("assets/audio/card.ogg"))
        exit(-1);
    if (!bufferDeerMode.loadFromFile("assets/audio/dp-deermode.ogg"))
        exit(-1);
    if (!bufferMeditation.loadFromFile("assets/audio/dp-meditation.ogg"))
        exit(-1);
    soundCollect.setBuffer(soundCollectBuffer);
    soundCollect.setVolume(20);

    soundCard.setBuffer(soundCardBuffer);
//    soundCollect.setVolume(20);

    if (!soundLetsBeginBuffer.loadFromFile("assets/audio/letsbegin.ogg"))
        exit(-1);
    soundLetsBegin.setBuffer(soundLetsBeginBuffer);
    soundLetsBegin.setVolume(20);

    soundDeerMode.setBuffer(bufferDeerMode);
    soundDeerMode.setVolume(40);
    soundMeditation.setBuffer(bufferMeditation);
    soundMeditation.setVolume(30);
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
