#include "boarddiamondseq.h"

BoardDiamondSeq::BoardDiamondSeq(TextureHolder *textures, int idNuber)
{
    this->textures = textures;
}

void BoardDiamondSeq::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    //    target.draw(animatedSprite, states);
}

void BoardDiamondSeq::update(sf::Time deltaTime, std::set<int> &busyTiles)
{

}


