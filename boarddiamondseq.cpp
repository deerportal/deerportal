#include "boarddiamondseq.h"

BoardDiamondSeq::BoardDiamondSeq(TextureHolder *textures)
{
    this->textures = textures;
    for (int i=0;  i<efc::diamondsNumber; i++)
    {
        diamonds[i] = BoardDiamond(this->textures,
                efc::DIAMONDS_SETUP[i][0],
                efc::DIAMONDS_SETUP[i][1],
                efc::DIAMONDS_SETUP[i][2]);
    }
    reorder();
}

void BoardDiamondSeq::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (int i=0;  i<efc::diamondsNumber; i++)
    {
        target.draw(diamonds[i],states);
    }
}

std::array<int,efc::diamondsNumber> BoardDiamondSeq::getBoardPositions()
{
    std::array<int, efc::diamondsNumber> results;
    for (int i=0;  i<efc::diamondsNumber; i++)
    {
        results[i] = diamonds[i].getBoardPosition();
    }
    return results;
}

bool BoardDiamondSeq::ifFieldIsEmpty(int pos, int element)
{
    for (int i=element*14;i<(efc::diamondsNumber/4)+(element*14);i++)
    {
        if (diamonds[i].boardPosition==pos)
            return false;
    }
    return true;

}

bool BoardDiamondSeq::ifFieldIsEmpty(int pos)
{
    for (int element=0;element<4;element++)
    {
        for (int i=element*14;i<(efc::diamondsNumber/4)+(element*14);i++)
        {
            if (diamonds[i].boardPosition==pos)
               return false;
         }
    }
    return true;

}

int BoardDiamondSeq::getRandomPos(int playerNumber)
{
    std::set<int> setSteps(efc::occupiedFields[playerNumber].cbegin(), efc::occupiedFields[playerNumber].cend());
    for (int i=playerNumber*14;i<(efc::diamondsNumber/4)+(playerNumber*14);i++)
    {
        if (diamonds[i].boardPosition!=-1)
        {
            setSteps.erase(diamonds[i].boardPosition);
        }
    }
    int step = rand()%setSteps.size();
    std::vector<int> freePositions;
    freePositions.resize(setSteps.size());
    freePositions.assign(setSteps.begin(),setSteps.end());
    return freePositions[step];
}

void BoardDiamondSeq::reorder()
{
    srand((unsigned)time(0));

    for (int element=0;element<4;element++)
    {
        int start = element;
        for (int i=start*14;i<(efc::diamondsNumber/4)+(start*14);i++)
        {
            int step = getRandomPos(element);
            diamonds[i].setBoardPosition(step);
        }

    }

}

void BoardDiamondSeq::reorder(int element)
{
    srand((unsigned)time(0));
    int start = element;
    if (element==2)
        start = 3;
    else if (element==3)
        start = 2;

    int newElement = start;

    for (int i=start*14;i<(efc::diamondsNumber/4)+(start*14);i++)
    {
        int step = getRandomPos(newElement);
        diamonds[i].setBoardPosition(step);
    }

}

void BoardDiamondSeq::collectField(int pos)
{
    for (int i=0;  i<efc::diamondsNumber; i++)
    {
        if (diamonds[i].boardPosition==pos)
        {
            diamonds[i].setBoardPosition(-1);
            break;
        }
    }
}

int BoardDiamondSeq::getNumberForField(int pos)
{
    for (int i=0;  i<efc::diamondsNumber; i++)
    {
        if (diamonds[i].boardPosition==pos)
        {
            return diamonds[i].idNumber;
        }
    }
    return -1;
}

