#include "cardsdeck.h"

CardsDeck::CardsDeck(TextureHolder *textures)
{


    std::array<std::array<int,2>,4> cardsPos = {
        {
            {1087,95}, {1225, 95}, {1225, 277}, {1087, 277}
    }
    };

    this->textures = textures;
    for (int i=0;i<=3;i++)
    {
        spriteCardBases[i].setTexture(this->textures->textureCardBases[i]);
        spriteCardBases[i].setPosition(cardsPos[i][0],cardsPos[i][1]);
    }




}

void CardsDeck::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (int i=0;i<=3;i++)
    {
        target.draw(spriteCardBases[i], states);
    }
}
