#include "rounddice.h"

#include "data.h"
#include "exceptions.h"
#include "filetools.h"
#include "textureholder.h"

RoundDice::RoundDice(Player (&players)[4]) : sfxDice(sfxDiceBuffer) {
  playersHud = players;
  diceResult = 1;
  diceResultSix = 6;
  diceSize = 150;
  if (!sfxDiceBuffer.loadFromFile(get_full_path(ASSETS_PATH "audio/dice.ogg"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, "audio/dice.ogg",
                                         "Failed to load dice sound effect");
  }

  if (!textureDice.loadFromFile(get_full_path(ASSETS_PATH "img/diceWhite.png"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/diceWhite.png", "Failed to load dice texture");
  }

  spriteDice = std::make_unique<sf::Sprite>(textureDice);
  sfxDice.setVolume(12);
  spriteDice->setPosition(sf::Vector2f(1140, 550));
  setDiceTexture();
}

void RoundDice::setDiceTexture() {
  sf::IntRect diceRect({diceSize * diceResultSix, 0}, {diceSize, diceSize});
  spriteDice->setTextureRect(diceRect);
}

void RoundDice::setColor(int playerNumber) {
  sf::Color color(DP::playersColors[playerNumber]);
  spriteDice->setColor(color);
}

void RoundDice::setDiceTexture(int diceResult) {
  this->diceResultSix = diceResult;
  setDiceTexture();
}

std::string RoundDice::drawRound() {
  throwDice();
  if (diceResult < 33) {
    return "end_of_round_extra_grow";
  } else if (diceResult < 66) {
    return "end_of_round_extra_energy";
  } else {
    // eventExtraCash();
    return "end_of_round_extra_cash";
  }
  return "end_of_round_" + std::to_string(diceResult);
}

int RoundDice::throwDice() {
  sfxDice.play();
  int result = rand() % 100;
  diceResult = result;
  return result;
}

int RoundDice::throwDiceSix() {
  sfxDice.play();
  int result = rand() % 6;
  diceResultSix = result;
  setDiceTexture();
  //    return 26;
  return result + 1;
}

void RoundDice::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (spriteDice) target.draw(*spriteDice, states);
}

void RoundDice::setFaces(int number) {
  // Face mapping: 0-5 = dice result faces 1-6, 6 = distinctive 45-degree waiting sprite (from
  // 0.8.2)
  this->diceResultSix = number;
  setDiceTexture();
}
