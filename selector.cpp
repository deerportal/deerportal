#include "selector.h"

Selector::Selector(int squareSize)
    :rectangle(sf::Vector2f(squareSize, squareSize))
{
    this->squareSize = squareSize;
    rectangle.setFillColor(sf::Color(150, 250, 150,168));

    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color(0,255,0));
    changeColor(3);
}

void Selector::changeColor(int colorNumber)
{

    if (colorNumber==0)
    {
        rectangle.setFillColor(sf::Color(150, 150, 250,168));
        rectangle.setOutlineColor(sf::Color(0,0,255));
    }
    else if (colorNumber==1)
    {
        rectangle.setFillColor(sf::Color(150, 250, 150,168));
        rectangle.setOutlineColor(sf::Color(0,255,0));
    }
    else if (colorNumber==2)
    {
        rectangle.setFillColor(sf::Color(250, 150, 150,168));
        rectangle.setOutlineColor(sf::Color(255,0,0));
    }
    else if (colorNumber==3)
    {
        rectangle.setFillColor(sf::Color(200, 200, 200,168));
        rectangle.setOutlineColor(sf::Color(200,200,200));
    }
}

void Selector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //   // apply the transform
    states.transform *= getTransform();
    target.draw(rectangle, states);
}
