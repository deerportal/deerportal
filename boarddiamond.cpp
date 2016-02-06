#include "boarddiamond.h"

BoardDiamond::BoardDiamond()
{
}

BoardDiamond::BoardDiamond(TextureHolder *textures, int idNumber,
                           int element, int boardPosition)
{
    this->textures = textures;
    this->element = element;
    this->boardPosition = boardPosition;
    spriteHolder.setTexture(this->textures->textureBoardDiamond);
    sf::IntRect textureRect(idNumber*44, 44, (idNumber*44)+idNumber,44);
    spriteHolder.setTextureRect(textureRect);
}

void BoardDiamond::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
//    target.draw(animatedSprite, states);
}

void BoardDiamond::update(sf::Time deltaTime, std::set<int> &busyTiles)
{

}
