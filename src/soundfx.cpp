#include "soundfx.h"

SoundFX::SoundFX()
{
    if (!soundCollectBuffer.loadFromFile(get_full_path("assets/audio/collect.ogg")))
        exit(-1);
    if (!soundCardBuffer.loadFromFile(get_full_path("assets/audio/card.ogg")))
        exit(-1);
    if (!bufferDeerMode.loadFromFile(get_full_path("assets/audio/dp-deermode.ogg")))
        exit(-1);
    if (!bufferMeditation.loadFromFile(get_full_path("assets/audio/dp-meditation.ogg")))
        exit(-1);
    if (!bufferPortal.loadFromFile(get_full_path("assets/audio/dp-ok.ogg")))
        exit(-1);




    soundCollect.setBuffer(soundCollectBuffer);
    soundCollect.setVolume(20);

    soundCard.setBuffer(soundCardBuffer);
//    soundCollect.setVolume(20);

    if (!soundLetsBeginBuffer.loadFromFile(get_full_path("assets/audio/letsbegin.ogg")))
        exit(-1);
    soundLetsBegin.setBuffer(soundLetsBeginBuffer);
    soundLetsBegin.setVolume(20);

    soundPortal.setBuffer(bufferPortal);
    soundPortal.setVolume(20);

    soundDeerMode.setBuffer(bufferDeerMode);
    soundDeerMode.setVolume(40);
    soundMeditation.setBuffer(bufferMeditation);
    soundMeditation.setVolume(20);
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
