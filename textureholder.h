#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class TextureHolder
{
public:
    TextureHolder();
    sf::Texture textureTiles;
    sf::Texture textureFaces;
    sf::Texture textureGui;


    std::map<int, std::map<int, int>> tilesDescription;
    std::map<int, std::string> tilesTxt;

};

#endif // TEXTUREHOLDER_H
