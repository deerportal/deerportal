#include "rotateelem.h"

#include <string> // For std::string

#include "data.h" // For ASSETS_PATH
#include "exceptions.h"
#include "textureholder.h"

RotateElem::RotateElem(TextureHolder* textures) : timeCounter(0), active(true) {
  if (!textureRotate.loadFromFile(get_full_path(std::string(ASSETS_PATH) + "img/rotate.png"))) {
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE, "img/rotate.png",
                                         "Failed to load rotate element texture");
  }

  spriteRotate = std::make_unique<sf::Sprite>(textureRotate);
  spriteRotate->scale(sf::Vector2f(0.7f, 0.7f));
  spriteRotate->setOrigin(sf::Vector2f(32.f, 32.f));
}

void RotateElem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  if (spriteRotate) target.draw(*spriteRotate, states);
}

void RotateElem::update(sf::Time deltaTime) {
  float speed;
  const float timestep = 1.0f / 60.0f;
  speed = 2.0f;
  float accumulator = 0;
  accumulator += deltaTime.asSeconds();
  if (spriteRotate) spriteRotate->rotate(sf::degrees(speed * accumulator / timestep));

  timeCounter += deltaTime.asSeconds();

  if (timeCounter > 0.40f) {
    if (active) {
      active = false;
    } else {
      active = true;
    };
    timeCounter = 0;
  }
}

void RotateElem::setColor() {
  if (spriteRotate) spriteRotate->setColor(sf::Color::White);
}

void RotateElem::reset() {
  active = true;
  timeCounter = 0;
}
