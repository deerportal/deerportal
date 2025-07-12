#ifndef GUIWINDOW_H
#define GUIWINDOW_H
#include <iostream>

#include <SFML/Graphics.hpp>

#include "hover.h"
#include "textureholder.h"

class GuiWindow : public Hover {
public:
  GuiWindow(sf::Font* gameFont);
  GuiWindow(TextureHolder* textures);
  GuiWindow();
  //    void setTextureHolder(TextureHolder *textures);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void init();
  virtual std::string getElem(sf::Vector2f mousePosition);
  sf::Font guiElemFont;
  std::string title;
  std::string description;
  std::unique_ptr<sf::Text> guiTitleTxt;
  //    sf::Texture* textureGui;

  std::unique_ptr<sf::Sprite> bgdDark;
  sf::Texture textureBgdDark;

  std::unique_ptr<sf::Sprite> spriteClose;
  sf::Texture textureClose;

  void setTitle(const std::string& newTitle);

private:
};

#endif // GUIWINDOW_H
