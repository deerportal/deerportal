#include "hover.h"

Hover::Hover()
{
    width = 150;
    height = 100;
    rectangle.setSize(sf::Vector2f(width, height));
    init();
}

Hover::Hover(int width, int height)
{
    this->width = width;
    this->height = height;
    rectangle.setSize(sf::Vector2f(width, height));
    init();
}

void Hover::init() {
    rectangle.setFillColor(sf::Color(230, 230, 233,218));
    rectangle.setOutlineColor(sf::Color(33,33,33));
    rectangle.setOutlineThickness(1);
    active = false;
}

void Hover::draw (sf::RenderTarget& target, sf::RenderStates states) const
{
        states.transform *= getTransform();
//    // apply the tileset texture
//    states.texture = m_tileset;

//    // draw the vertex array
        if (active==true)
            target.draw(rectangle, states);
}


