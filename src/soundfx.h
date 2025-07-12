#ifndef SOUNDFX_H
#define SOUNDFX_H
#include <iostream>

#include <SFML/Audio.hpp>

#include "filetools.h"
/*!
 * \brief The SoundFX various sounds
 */
class SoundFX {
public:
  SoundFX();

  sf::SoundBuffer soundCollectBuffer;
  sf::SoundBuffer soundCardBuffer;
  sf::SoundBuffer bufferDeerMode;
  sf::SoundBuffer bufferMeditation;
  sf::SoundBuffer bufferPortal;
  sf::Sound soundPortal;
  sf::Sound soundDeerMode;
  sf::Sound soundMeditation;
  sf::Sound soundCollect;
  sf::Sound soundCard;
  void playCollect();

  sf::SoundBuffer soundLetsBeginBuffer;
  sf::Sound soundLetsBegin;
  void playLetsBegin();
  void playCard();
};

#endif // SOUNDFX_H
