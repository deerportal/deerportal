#ifndef CARDSDECK_H
#define CARDSDECK_H
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "textureholder.h"
#include "cardslist.h"
#include "command.h"




class CardsDeck: public sf::Drawable, public sf::Transformable
{
public:
    CardsDeck(TextureHolder *textures, sf::Font *gameFont, Command *command);
    std::array<CardsList, 4> cardsList;
    std::array<sf::Sprite, 4> spriteCardBases;
    std::array<sf::Text, 4> textPileTitle;
    TextureHolder *textures;

    Command *commandManager;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
//    void setTitles();
    void setFonts(sf::Font *gameFont);
    void nextCard(int pileNumber);
    int getCurrentCard(int pileNumber);
    std::string getTitle(int pileNumber);

    void update(sf::Time deltaTime);
    int getCardTypeInt(int pileNumber);
    void setTitles(int number);
    void reloadCards();
};

#endif // CARDSDECK_H

