#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H
#include <set>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
namespace efc {
static int terrain[24] = {
    8, 24, 40, 56, 72, 88,
    113, 114, 115, 116, 117, 118,
    138, 139, 140, 141, 142, 143,
    167, 183, 199, 215, 231, 247
};


static std::set<int> getTerrainSet() {
    std::set<int> terrain;
    for (int i: efc::terrain)
    {
        terrain.insert(i);
    }
    return terrain;
}

}



class TextureHolder
{
public:
    TextureHolder();
    sf::Texture textureTiles;
    sf::Texture textureFaces;
    sf::Texture textureGui;
    sf::Texture textureMenu;
    sf::Texture textureSymbols;

    sf::Texture backgroundDark;



    std::map<int, std::map<int, int>> tilesDescription;
    std::map<int, std::string> tilesTxt;

};

#endif // TEXTUREHOLDER_H
