#include "playerhud.h"

PlayerHud::PlayerHud(int faceNumber, sf::Texture *faces)
{

}

void PlayerHud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
//   // apply the transform
   states.transform *= getTransform();


    sf::RectangleShape rectangle(sf::Vector2f(25, 25));
    rectangle.setFillColor(sf::Color(50, 50, 150,168));
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color(0,0,128));

    target.draw(rectangle, states);


}
