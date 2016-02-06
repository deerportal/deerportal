#ifndef BOARDDIAMONDSEQ_H
#define BOARDDIAMONDSEQ_H

#include <set>
#include "textureholder.h"
#include <SFML/Graphics.hpp>
#include "boarddiamond.h"



/*!
 * \brief The BoardDiamondSeq class
 * We are going to have 2 cards / diamonds of each element per each area,
 * which gives 2*4*4 = 32 cards diamonds of elements on the board.
 *
 * Additionally there will be extra 6 white diamonds per area to collect, which
 * gives 4*6 = 24 diamonds on the board.
 *
 * Together it would give 32 + 24 = 60 diamonds cards/diamonds together,
 * 15 per area.
 */

class BoardDiamondSeq : public sf::Drawable, public sf::Transformable
{
public:
    BoardDiamondSeq(TextureHolder *textures);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime, std::set<int>& busyTiles);
    TextureHolder *textures;

    const static int DIAMONDS_NUMBER = 4;
    BoardDiamond diamonds[DIAMONDS_NUMBER];
};



#endif // BOARDDIAMONDSEQ_H
