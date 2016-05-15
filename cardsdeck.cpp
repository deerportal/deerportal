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
//        spriteCardBases[i].setTexture(this->textures->textureCardBases[i]);
        spriteCardBases[i].setPosition(cardsPos[i][0],cardsPos[i][1]);

        textPileTitle[i].setFont(*gameFont);
        textPileTitle[i].setCharacterSize(10);
        textPileTitle[i].setPosition(cardsPos[i][0]+10,cardsPos[i][1]+100);

        for (int j=0;j<3;j++)
        {
            std::cout << j << std::endl;

            cardsList[i].cardsPile[j].cardType = efc::cardsTypes[j];
            cardsList[i].cardsPile[j].cardTypeInt = j;
            spriteCardBases[i].setTexture(this->textures->cardsTextures[i][j]);

        }

        cardsList[i].shufflePile();
        nextCard(i);

//        setTitles();


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

void CardsDeck::setTitles(int number)
{

        textPileTitle[number].setString(efc::cardsTypes[number]);
        int val = cardsList[number].currentCard;
        std::cout <<number<< " << hjehe >> " << getTitle(number) << " " << val << std::endl;

        spriteCardBases[number].setTexture(textures->cardsTextures[number][val]);

}

void CardsDeck::setSprites(int number)
{
//    for (int i=0;i<=3;i++)
//    {
//        int val = getCardTypeInt(i);
//        std::string title = getTitle(i);
//        std::cout <<i<< " << hjehe >> " << val << std::endl;

//        sf::Texture tmpText = textures->cardsTextures[i][val];
//        spriteCardBases[i].setTexture(tmpText);
//    }
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
    std::cout << currentCard << " ccard" << std::endl;
    if ((currentCard>3) || (currentCard<0))
        currentCard = 0;
    cardsList[pileNumber].currentCard = currentCard;
    setTitles(pileNumber);
//    setSprites(pileNumber);
}

int CardsDeck::getCurrentCard(int pileNumber)
{
    int currentCard = cardsList[pileNumber].currentCard;
    return currentCard;
}

std::string CardsDeck::getTitle(int pileNumber)
{
    std::string currentText = efc::cardsTypes[getCardTypeInt(pileNumber)];
    return currentText;
}

int CardsDeck::getCardTypeInt(int pileNumber)
{
    int result = cardsList[pileNumber].cardsPile[getCurrentCard(pileNumber)].cardTypeInt;
    return result;
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
