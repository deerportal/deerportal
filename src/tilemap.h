#ifndef TILEMAP_H
#define TILEMAP_H
#include <iostream>
#include <set>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "textureholder.h"

namespace DP {
enum { TILE_SIZE = 40, BOARD_SIZE = 16, TILE_BOARD_SIZE = 80 };
sf::Vector2i transPosition(int pos);
int transCords(sf::Vector2i cords);
std::set<int> getNeighbours(int pos);
sf::IntRect transPosIntoRect(int pos);
sf::Vector2i transTilePosition(int pos);

sf::Vector2i getCords(sf::Vector2f position);
sf::Vector2f getScreenPos(sf::Vector2i cords);
} // namespace DP

class TileMap : public sf::Drawable, public sf::Transformable {
public:
  //    bool load(TextureHolder *textures, sf::Vector2u tileSize, const int* tiles, unsigned int
  //    width, unsigned int height);
  TileMap();

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::VertexArray m_vertices;
  sf::Texture* m_tileset;
  TextureHolder* textures;
};

#endif // TILEMAP_H
