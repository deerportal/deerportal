#ifndef HOVER_H
#define HOVER_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Hover : public sf::Drawable, public sf::Transformable {
public:
  Hover();
  Hover(int width, int height);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  sf::RectangleShape rectangle;
  int width;
  int height;

  void init();

  bool active;
};

#endif // HOVER_H
