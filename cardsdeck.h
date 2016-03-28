#ifndef CARDSDECK_H
#define CARDSDECK_H
#include <array>
#include <SFML/Graphics.hpp>
#include "textureholder.h"
#include "cardslist.h"



class CardsDeck: public sf::Drawable, public sf::Transformable
{
public:
    CardsDeck(TextureHolder *textures);
    std::array<CardsList, 4> cardsList;
    std::array<sf::Sprite, 4> spriteCardBases;
    TextureHolder *textures;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // CARDSDECK_H
