#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H
#include <array>
#include <map>
#include <set>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "data.h"
#include "filetools.h"

namespace DP {

std::set<int> getTerrainSet();

}

/*!
 * \brief The TextureHolder contains most textures.
 */
class TextureHolder {

public:
  TextureHolder();
  //    sf::Texture textureTiles;
  //    sf::Texture textureFaces;
  //    sf::Texture textureGui;
  sf::Texture textureMenu;
  //    sf::Texture textureSymbols;
  //    sf::Texture textureSeasons;
  sf::Texture backgroundDark;
  sf::Texture textureCharacters;
  //    sf::Texture textureGameBackground;
  sf::Texture textureBoardDiamond;
  sf::Texture textureLetsBegin;

  sf::Texture textureCardBase0;
  sf::Texture textureCardBase1;
  sf::Texture textureCardBase2;
  sf::Texture textureCardBase3;

  sf::Texture textureButtonCpu;
  sf::Texture textureButtonHuman;

  sf::Texture textureDeerGod;

  std::array<sf::Texture, 4> textureCardBases;

  std::map<int, std::map<int, int>> tilesDescription;
  std::map<int, std::string> tilesTxt;

  std::array<std::array<sf::Texture, 4>, 4> cardsTextures;

  sf::Texture textureBigDiamond;
};

#endif // TEXTUREHOLDER_H
