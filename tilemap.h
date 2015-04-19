#ifndef TILEMAP_H
#define TILEMAP_H
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "textureholder.h"
namespace efc {
enum {
    TILE_SIZE = 25,
    BOARD_SIZE = 16,
    TILE_BOARD_SIZE = 40
};
sf::Vector2i transPosition(int pos);
int transCords(sf::Vector2i cords);
std::vector<int>  getNeighbours(int pos);
}

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(TextureHolder *textures, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
    TileMap();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray m_vertices;
    sf::Texture *m_tileset;
    TextureHolder *textures;


};

#endif // TILEMAP_H
