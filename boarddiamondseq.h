#ifndef BOARDDIAMONDSEQ_H
#define BOARDDIAMONDSEQ_H

#include <set>
#include "textureholder.h"
#include <SFML/Graphics.hpp>

class BoardDiamondSeq : public sf::Drawable, public sf::Transformable
{
public:
    BoardDiamondSeq(TextureHolder *textures, int idNumber);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime, std::set<int>& busyTiles);
    TextureHolder *textures;
    int idNumber;
};



#endif // BOARDDIAMONDSEQ_H
