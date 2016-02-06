#include "soundfx.h"

SoundFX::SoundFX()
{
    if (!soundCollectBuffer.loadFromFile("assets/audio/collect.wav"))
        exit(-1);
    soundCollect.setBuffer(soundCollectBuffer);

}

void SoundFX::playCollect()
{
    soundCollect.play();
}
