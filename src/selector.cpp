#include "selector.h"

Selector::Selector(int squareSize) : rectangle(sf::Vector2f(squareSize - 1, squareSize - 1)) {
  this->squareSize = squareSize;
  rectangle.setFillColor(sf::Color(150, 250, 150, 168));

  //    rectangle./*setOutlineThickness*/(1);
  rectangle.setOutlineColor(sf::Color(0, 255, 0));
  changeColor(3);
}

void Selector::changeColor(int colorNumber) {
  sf::Color color(DP::playersColors[colorNumber]);
  color.a = 128;
  rectangle.setFillColor(color);
}

void Selector::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  //   // apply the transform
  states.transform *= getTransform();
  target.draw(rectangle, states);
}
