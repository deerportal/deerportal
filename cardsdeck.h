#ifndef CARDSDECK_H
#define CARDSDECK_H
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "textureholder.h"
#include "cardslist.h"

namespace efc {
const static std::array<std::string, efc::PILE_SIZE> cardsTypes = {
    "stop", "card", "diamond"
};
}

class CardsDeck: public sf::Drawable, public sf::Transformable
{
public:
    CardsDeck(TextureHolder *textures, sf::Font *gameFont);
    std::array<CardsList, 4> cardsList;
    std::array<sf::Sprite, 4> spriteCardBases;
    std::array<sf::Text, 4> textPileTitle;
    TextureHolder *textures;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void setTitles();
    void setFonts(sf::Font *gameFont);
    void nextCard(int pileNumber);
    int getCurrentCard(int pileNumber);
    std::string getTitle(int pileNumber);

    void update(sf::Time deltaTime);
};

#endif // CARDSDECK_H
