#include "playerhud.h"

PlayerHud::PlayerHud(int faceNumber, sf::Texture *faces, int tileSize, int pos)
{
    this->tileSize = tileSize;
    spriteFace.setTexture(*faces);
    this->pos = pos;



    int y = (int) faceNumber /10;
    int x = faceNumber % 10;


    std::cout << "playerHud" << faceNumber << " " << x << " " << y << std::endl;

    spriteFace.setTextureRect(sf::IntRect(x*tileSize, y*tileSize, tileSize, tileSize));
    spriteFace.setScale(sf::Vector2f(0.25f, 1.f));
    spriteFace.setPosition(0,pos*100);
}

void PlayerHud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
//   // apply the transform
   states.transform *= getTransform();


    sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));

    if (pos==0)
       rectangle.setFillColor(sf::Color(50, 50, 150,168));
    else if (pos==1)
       rectangle.setFillColor(sf::Color(50, 150, 50,168));
    else if (pos==2)
       rectangle.setFillColor(sf::Color(150, 50, 50,168));
    else if (pos==3)
       rectangle.setFillColor(sf::Color(150, 150, 150,168));


    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color(0,0,128));
    rectangle.setPosition(0, pos*100);
    target.draw(rectangle, states);
    target.draw(spriteFace);


}
