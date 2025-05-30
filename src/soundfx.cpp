#include "soundfx.h"

SoundFX::SoundFX()
    : soundPortal(bufferPortal),
      soundDeerMode(bufferDeerMode), 
      soundMeditation(bufferMeditation),
      soundCollect(soundCollectBuffer),
      soundCard(soundCardBuffer),
      soundLetsBegin(soundLetsBeginBuffer)
{
    if (!soundCollectBuffer.loadFromFile(get_full_path(ASSETS_PATH"assets/audio/collect.ogg")))
        exit(-1);
    if (!soundCardBuffer.loadFromFile(get_full_path(ASSETS_PATH"assets/audio/card.ogg")))
        exit(-1);
    if (!bufferDeerMode.loadFromFile(get_full_path(ASSETS_PATH"assets/audio/dp-deermode.ogg")))
        exit(-1);
    if (!bufferMeditation.loadFromFile(get_full_path(ASSETS_PATH"assets/audio/dp-meditation.ogg")))
        exit(-1);
    if (!bufferPortal.loadFromFile(get_full_path(ASSETS_PATH"assets/audio/dp-ok.ogg")))
        exit(-1);

    if (!soundLetsBeginBuffer.loadFromFile(get_full_path(ASSETS_PATH"assets/audio/letsbegin.ogg")))
        exit(-1);

    // Set volume for sounds (buffers already set in initializer list)
    soundCollect.setVolume(20);
    soundCard.setVolume(20); // Fixed missing volume
    soundLetsBegin.setVolume(20);
    soundPortal.setVolume(20);
    soundDeerMode.setVolume(40);
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
