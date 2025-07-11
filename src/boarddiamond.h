#ifndef BOARDDIAMOND_H
#define BOARDDIAMOND_H
#include <memory>
#include <set>

#include <SFML/Graphics.hpp>

#include "elem.h"
#include "textureholder.h"

class BoardDiamond : public Elem {
public:
  BoardDiamond();
  BoardDiamond(TextureHolder* textures, int idNumber, int playerNumber, int boardPosition);

  TextureHolder* textures;
  /*!
   * \brief number from the sprite sheet
   *
   * 0-3 are corresponding to the players numbers,
   * everything above are the extra items:
   * 4 - diamond
   */
  int idNumber;

  /*!
   * \brief 0-3 elements area on the board
   */
  int playerNumber;
  std::unique_ptr<sf::Sprite> spriteHolder;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  //    void update(sf::Time deltaTime, std::set<int>& busyTiles);
};

#endif // BOARDDIAMOND_H
