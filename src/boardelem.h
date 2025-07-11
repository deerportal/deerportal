#ifndef BOARDELEM_H
#define BOARDELEM_H
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "textureholder.h"
#include "tilemap.h"

namespace DP {
sf::RectangleShape createNeighbour(int pos);

class BoardElem : public sf::Sprite {
public:
  BoardElem();
  BoardElem(int pos, int type);

  static sf::Texture& getDefaultTexture();

  int pos;
  int type;

  sf::Texture* textureTiles;
};
} // namespace DP

#endif // BOARDELEM_H
