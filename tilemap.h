#ifndef TILEMAP_H
#define TILEMAP_H
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
namespace efc {
enum {
    TILE_SIZE = 25
};
}
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(sf::Texture *m_tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
    TileMap();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray m_vertices;
    sf::Texture *m_tileset;

};

#endif // TILEMAP_H
