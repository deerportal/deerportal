#include "bubble.h"
#include "exceptions.h"

#include <array>
#include <cmath>

Bubble::Bubble() : state(BubbleState::DICE), timeCounter(0), posY(0) {
  if (!textureDice.loadFromFile(get_full_path(ASSETS_PATH "img/bubble_dice.png"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE, 
                                       "img/bubble_dice.png", 
                                       "Failed to load bubble dice texture");
  }

  if (!textureFootSteps.loadFromFile(get_full_path(ASSETS_PATH "img/bubble_footsteps.png"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE, 
                                       "img/bubble_footsteps.png", 
                                       "Failed to load bubble footsteps texture");
  }

  spriteDice = std::make_unique<sf::Sprite>(textureDice);
  spriteFootSteps = std::make_unique<sf::Sprite>(textureFootSteps);

  spritesBubbles[0] = std::make_unique<sf::Sprite>(textureDice);
  spritesBubbles[1] = std::make_unique<sf::Sprite>(textureFootSteps);
}

void Bubble::setPosition(float x, float y) {
  Transformable::setPosition(sf::Vector2f(x, y));
  posY = y;
}

void Bubble::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(*spritesBubbles[state], states);
}

void Bubble::update(sf::Time deltaTime) {

  timeCounter += deltaTime.asSeconds() * 10;
  float modifier = sin(timeCounter) * 5;
  Transformable::setPosition(sf::Vector2f(getPosition().x, posY + modifier));
}
