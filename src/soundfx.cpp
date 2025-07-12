#include "soundfx.h"
#include "exceptions.h"

SoundFX::SoundFX()
    : soundPortal(bufferPortal), soundDeerMode(bufferDeerMode), soundMeditation(bufferMeditation),
      soundCollect(soundCollectBuffer), soundCard(soundCardBuffer),
      soundLetsBegin(soundLetsBeginBuffer) {
  if (!soundCollectBuffer.loadFromFile(get_full_path(ASSETS_PATH "audio/collect.ogg"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/collect.ogg", 
                                       "Failed to load collect sound effect");
  }
  if (!soundCardBuffer.loadFromFile(get_full_path(ASSETS_PATH "audio/card.ogg"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/card.ogg", 
                                       "Failed to load card sound effect");
  }
  if (!bufferDeerMode.loadFromFile(get_full_path(ASSETS_PATH "audio/dp-deermode.ogg"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/dp-deermode.ogg", 
                                       "Failed to load deer mode sound effect");
  }
  if (!bufferMeditation.loadFromFile(get_full_path(ASSETS_PATH "audio/dp-meditation.ogg"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/dp-meditation.ogg", 
                                       "Failed to load meditation sound effect");
  }
  if (!bufferPortal.loadFromFile(get_full_path(ASSETS_PATH "audio/dp-ok.ogg"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/dp-ok.ogg", 
                                       "Failed to load portal sound effect");
  }

  if (!soundLetsBeginBuffer.loadFromFile(get_full_path(ASSETS_PATH "audio/letsbegin.ogg"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/letsbegin.ogg", 
                                       "Failed to load lets begin sound effect");
  }

  // Set volume for sounds (buffers already set in initializer list)
  soundCollect.setVolume(20);
  soundCard.setVolume(20); // Fixed missing volume
  soundLetsBegin.setVolume(20);
  soundPortal.setVolume(20);
  soundDeerMode.setVolume(40);
  soundMeditation.setVolume(20);
}

void SoundFX::playCollect() {
  soundCollect.play();
}

void SoundFX::playCard() {
  soundCard.play();
}

void SoundFX::playLetsBegin() {
  //    std::cout << "playing.... begin lets" << std::endl;
  soundLetsBegin.play();
}
