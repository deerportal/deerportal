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
    sf::IntRect textureRect(idNumber*44, 0, 44,44);
    spriteHolder.setTextureRect(textureRect);
    setBoardPosition(boardPosition);
}

void BoardDiamond::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (boardPosition>-1)
    {
        states.transform *= getTransform();
        sf::Vector2f where(getPosition());
        std::cout << "drawing " << where.x << " " << where.y << std::endl;
        target.draw(spriteHolder, states);
    }
}

void BoardDiamond::update(sf::Time deltaTime, std::set<int> &busyTiles)
{

}
