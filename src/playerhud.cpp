#include "playerhud.h"

#include <iostream>
#include <string>

#include "boardelem.h"
#include "textureholder.h"

std::set<int> Player::getTerrainSet() {
  std::set<int> terrain;
  for (int i : DP::terrainArray) {
    terrain.insert(i);
  }
  return terrain;
}

std::set<int> Player::getBusy() {
  std::set<int> busyTiles;
  for (const auto& pair : elems.items_map) {
    busyTiles.insert(pair.first);
  }
  return busyTiles;
}

std::set<int> Player::getNeighbours() {
  std::set<int> neighbours;
  for (const auto& pair : elems.items_map) {
    std::set<int> terrain = getTerrainSet();
    std::set<int> neighboursVector(DP::getNeighbours(pair.second.pos));
    for (int j : neighboursVector) {
      if ((elems.items_map.count(j) == 0) && (terrain.count(j) == 0)) {
        neighbours.insert(j);
      }
    }
  }
  return neighbours;
}

void Player::updateTxt() {
  if (txtCash) txtCash->setString(std::to_string(cash));
  if (txtFood) txtFood->setString(std::to_string(food));
  if (txtEnergy) txtEnergy->setString(std::to_string(energy));
  if (txtFaith) txtFaith->setString(std::to_string(faith));
}

void Player::updatePlayer() {
  updateTxt();
}

Player::Player()
    : frozenLeft(0), reachedPortal(false), reachedPortalFirst(false), human(false),
      reachPortalMode(false) {
  pos = 0;
  cash = 0;
  energy = 0;
  food = 0;
  faith = 0;
  active = false;
  done = false;
  tileSize = 0;
  textures = nullptr;
  number = 0;
}

void Player::setActive(bool newState) {
  active = newState;
  elems.active = newState;
  for (auto& i : characters) {
    i.active = newState;
  }
}

Player::Player(TextureHolder* textures, sf::Font* gameFont, int playerNumber)
    : reachedPortal(false), reachedPortalFirst(false), human(false), reachPortalMode(false) {
  number = playerNumber;

  frozenLeft = 0;
  done = false;

  active = false;
  this->textures = textures;
  characters.push_back(Character(this->textures, playerNumber));

  this->pos = playerNumber;

  food = 0;
  cash = 0;
  energy = 0;
  faith = 0;

  txtCash = std::make_unique<sf::Text>(*gameFont);
  txtCash->setCharacterSize(20);
  txtCash->setFillColor(sf::Color(200, 200, 200, 180));

  txtFood = std::make_unique<sf::Text>(*gameFont);
  txtEnergy = std::make_unique<sf::Text>(*gameFont);
  txtFaith = std::make_unique<sf::Text>(*gameFont);

  txtNextRound = std::make_unique<sf::Text>(*gameFont);
  txtNextRound->setString("End Turn");
  txtNextRound->setCharacterSize(12);
  txtNextRound->setPosition(sf::Vector2f(40.f, static_cast<float>((playerNumber * 100) + 10)));

  int posX1 = 82;
  int posX2 = 962;
  int posY1 = 22;
  int posY2 = 720;
  std::array<std::array<int, 2>, 4> textPos = {
      {{{posX1, posY1}}, {{posX2, posY1}}, {{posX1, posY2}}, {{posX2, posY2}}}};

  txtCash->setPosition(sf::Vector2f(static_cast<float>(textPos[playerNumber][0]),
                                    static_cast<float>(textPos[playerNumber][1])));
  buttons.insert({"end_turn", rectangle});

  // Initialize sprite unique_ptrs with available textures
  spriteAI = std::make_unique<sf::Sprite>(textures->textureButtonCpu);
  // symbol = std::make_unique<sf::Sprite>(textures->textureBigDiamond); // COMMENTED OUT - This was
  // causing diamond to appear in wrong place
  setSpriteAI();
}

std::string Player::getElem(sf::Vector2f mousePosition) {
  std::string result = "";
  sf::Vector2f hoverPos = getPosition();
  for (const auto& pair : buttons) {
    sf::FloatRect spriteBounds = pair.second.getLocalBounds();
    sf::FloatRect closeRect;
    closeRect.position.x = pair.second.getPosition().x;
    closeRect.position.y = pair.second.getPosition().y;
    closeRect.size.x = spriteBounds.size.x;
    closeRect.size.y = spriteBounds.size.y;

    sf::Vector2f relativeMousePos(mousePosition.x - hoverPos.x, mousePosition.y - hoverPos.y);
    if (closeRect.contains(relativeMousePos)) {
      return pair.first;
    }
  }
  return result;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  if (txtCash) target.draw(*txtCash, states);
  // if (txtEnergy) target.draw(*txtEnergy, states);  // REMOVED - Not drawn in 0.8.2
  // if (txtFood) target.draw(*txtFood, states);      // REMOVED - Not drawn in 0.8.2
  // if (txtFaith) target.draw(*txtFaith, states);    // REMOVED - Not drawn in 0.8.2
  // if (txtNextRound) target.draw(*txtNextRound, states); // REMOVED - Not drawn in 0.8.2
  // if (symbol) target.draw(*symbol, states); // COMMENTED OUT - was using wrong texture
}

void Player::play() {
  for (auto& i : characters) {
    i.play();
  }
}

void Player::update(sf::Time deltaTime) {
  updateTxt();
  if (frozenLeft == 0) {
    for (auto& i : characters) {
      sf::Vector2f movement(0.f, 0.f);
      if (i.currentAnimationIndex == DP::DIR_LEFT)
        movement = sf::Vector2f(-10.f, 0.f);
      else if (i.currentAnimationIndex == DP::DIR_RIGHT)
        movement = sf::Vector2f(10.f, 0.f);
      else if (i.currentAnimationIndex == DP::DIR_UP)
        movement = sf::Vector2f(0.f, -10.f);
      else if (i.currentAnimationIndex == DP::DIR_DOWN)
        movement = sf::Vector2f(0.f, 10.f);
      i.update(deltaTime);
    }
  } else {
    characters[0].currentAnimationIndex = DP::DIR_DOWN;
  }
}

std::array<int, 2> Player::getMovements(int diceResult) {
  return characters[0].getMovements(diceResult);
}

void Player::setFigurePos(int pos) {
  characters[0].setBoardPosition(pos);
}

void Player::restartPlayer() {
  food = 0;
  cash = 0;
  energy = 0;
  faith = 0;
}

void Player::setHuman(bool newHuman) {
  human = newHuman;
  setSpriteAI();
}

void Player::swapHuman() {
  if (human) {
    setHuman(false);
    return;
  } else {
    setHuman(true);
    return;
  }
}

void Player::setSpriteAI() {
  if (!spriteAI || !textures) return;

  if (human)
    spriteAI->setTexture(textures->textureButtonHuman);
  else
    spriteAI->setTexture(textures->textureButtonCpu);

  std::array<std::array<int, 2>, 4> spriteHumanPos = {
      {{{220, 450}}, {{1050, 450}}, {{140, 600}}, {{1110, 600}}}};

  sf::Vector2f spriteHumanPosNew(static_cast<float>(spriteHumanPos[number][0]),
                                 static_cast<float>(spriteHumanPos[number][1]));
  spriteAI->setPosition(spriteHumanPosNew);
}
