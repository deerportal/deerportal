#ifndef PLAYERHUD_H
#define PLAYERHUD_H
#include <iostream>
#include <set>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "boardelems.h"
#include "character.h"
#include "data.h"
#include "elemsdescription.h"
#include "guiwindow.h"
#include "textureholder.h"

class Player : public sf::Drawable, public sf::Transformable {
public:
  Player();
  Player(TextureHolder* textures, sf::Font* gameFont, int pos);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  std::set<int> getNeighbours();
  int pos;
  int cash;
  int energy;
  int food;
  int faith;
  bool active;
  BoardElems elems;
  void setActive(bool newState);
  std::vector<int> properties;

  void updatePlayer();
  std::string getElem(sf::Vector2f mousePosition);
  std::map<std::string, sf::RectangleShape> buttons;
  void updateTxt();

  std::vector<Character> characters;

  void update(sf::Time deltaTime);
  void play();

  std::set<int> getBusy();
  std::array<int, 2> getMovements(int diceResult);
  void setFigurePos(int pos);
  bool done;

  int frozenLeft;

  void restartPlayer();

  bool reachedPortal;
  bool reachedPortalFirst;

  bool human;

  sf::Texture textureAI;
  std::unique_ptr<sf::Sprite> spriteAI;
  sf::Texture textureCountries[4];
  std::map<sf::Keyboard::Key, sf::RectangleShape> elemKeys;

  /*
   * This is 4 different players areas on the screen
   */
  //    sf::Sprite spriteFace;

  std::unique_ptr<sf::Text> txtCash;
  std::unique_ptr<sf::Text> txtEnergy;
  std::unique_ptr<sf::Text> txtFood;
  std::unique_ptr<sf::Text> txtFaith;
  std::unique_ptr<sf::Text> txtNextRound;
  sf::Color colorMain;
  sf::Color colorSecond;
  sf::Font* gameFont;

  bool aiActive;
  std::unique_ptr<sf::Sprite> symbol;
  sf::Texture symbolTexture;

  int tileSize;
  TextureHolder* textures;
  sf::RectangleShape rectangle;
  sf::RectangleShape rectangle2;

  void setHuman(bool newHuman);

  int number;

  void swapHuman();
  void setSpriteAI();
  bool reachPortalMode;

private:
  //    sf::Sprite spriteFace;
  std::set<int> getTerrainSet();
};

#endif // PLAYERHUD_H
