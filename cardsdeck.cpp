#include "cardsdeck.h"

CardsDeck::CardsDeck(TextureHolder *textures, sf::Font *gameFont)
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

        textPileTitle[i].setFont(*gameFont);
        textPileTitle[i].setCharacterSize(10);
        textPileTitle[i].setPosition(cardsPos[i][0]+10,cardsPos[i][1]+100);

        for (int j=0;j<efc::PILE_SIZE;j++)
        {
            cardsList[i].cardsPile[j].cardType = efc::cardsTypes[j];
        }
        setTitles();
        cardsList[i].shufflePile();
    }
}

void CardsDeck::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (int i=0;i<=3;i++)
    {
        if (cardsList[i].invisibleLeft==0.0f)
        {
            target.draw(spriteCardBases[i], states);
            target.draw(textPileTitle[i], states);
        }
    }
}

void CardsDeck::setTitles()
{
    for (int i=0;i<=3;i++)
    {
        textPileTitle[i].setString(getTitle(i));
    }
}

void CardsDeck::setFonts(sf::Font *gameFont)
{
    for (int i=0;i<=3;i++)
    {
        textPileTitle[i].setFont(*gameFont);
    }
}

void CardsDeck::nextCard(int pileNumber)
{
    cardsList[pileNumber].invisibleLeft = 0.75f;
    int currentCard = getCurrentCard(pileNumber);
    currentCard += 1;
    if (currentCard>efc::PILE_SIZE-1)
        currentCard = 0;
    cardsList[pileNumber].currentCard = currentCard;
    setTitles();
}

int CardsDeck::getCurrentCard(int pileNumber)
{
    int currentCard = cardsList[pileNumber].currentCard;
    return currentCard;
}

std::string CardsDeck::getTitle(int pileNumber)
{
    std::string currentText = cardsList[pileNumber].cardsPile[getCurrentCard(pileNumber)].cardType;
    return currentText;
}


void CardsDeck::update(sf::Time deltaTime)
{
    for (int i=0;i<=3;i++)
    {
        if (cardsList[i].invisibleLeft>0.0f)
        {
            cardsList[i].invisibleLeft -= deltaTime.asSeconds();

        }
        if (cardsList[i].invisibleLeft<0.0f)
        {
            cardsList[i].invisibleLeft = 0.0f;
        }
    }

}
