#include "boarddiamondseq.h"

BoardDiamondSeq::BoardDiamondSeq(TextureHolder *textures)
{
    this->textures = textures;

    diamonds[0] = BoardDiamond(textures, 0,0, 5);
    diamonds[1] = BoardDiamond(textures, 0,0, 6);
    diamonds[2] = BoardDiamond(textures, 1,0, 7);
    diamonds[3] = BoardDiamond(textures, 1,0, 17);

}

void BoardDiamondSeq::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (int i=1;  i<DIAMONDS_NUMBER; i++)
    {

        target.draw(diamonds[i],states);
    }
}

void BoardDiamondSeq::update(sf::Time deltaTime, std::set<int> &busyTiles)
{

}


