#include "boardelem.h"

namespace DP {

sf::Texture& BoardElem::getDefaultTexture() {
  static sf::Texture defaultTexture;
  static bool initialized = false;
  if (!initialized) {
    if (!defaultTexture.resize(sf::Vector2u(1, 1))) {
      throw std::runtime_error("Failed to resize default texture for BoardElem");
    }
    initialized = true;
  }
  return defaultTexture;
}

sf::RectangleShape createNeighbour(int pos) {

  sf::RectangleShape rectangle(sf::Vector2f(DP::TILE_SIZE, DP::TILE_SIZE));
  sf::Vector2i cords(DP::transPosition(pos));

  rectangle.setFillColor(sf::Color(240, 240, 240, 98));
  rectangle.setOutlineColor(sf::Color(24, 24, 24, 90));

  rectangle.setPosition(sf::Vector2f(cords.x * DP::TILE_SIZE, cords.y * DP::TILE_SIZE));
  return rectangle;
}
} // namespace DP

DP::BoardElem::BoardElem(int pos, int type) : sf::Sprite(getDefaultTexture()) {
  this->pos = pos;
  this->type = type;
  sf::Vector2i cords = DP::transPosition(pos);
  int y_type = (int)type / DP::TILE_BOARD_SIZE;
  int x_type = type % DP::TILE_BOARD_SIZE;
  setPosition(sf::Vector2f(cords.x * DP::TILE_SIZE, cords.y * DP::TILE_SIZE));
  setTextureRect(sf::IntRect(sf::Vector2i(x_type * DP::TILE_SIZE, y_type * DP::TILE_SIZE),
                             sf::Vector2i(DP::TILE_SIZE, DP::TILE_SIZE)));
  textureTiles = nullptr;
}
