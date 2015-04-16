#include "selector.h"

Selector::Selector(int squareSize)
{
    this->squareSize = squareSize;

}


void Selector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
//   // apply the transform
   states.transform *= getTransform();

//   // apply the tileset texture
//   states.texture = &m_tileset;

//   // draw the vertex array
//   target.draw(m_vertices, states);


    sf::RectangleShape rectangle(sf::Vector2f(squareSize, squareSize));
    rectangle.setFillColor(sf::Color(150, 250, 150,168));
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color(0,255,0));

    target.draw(rectangle, states);


}
