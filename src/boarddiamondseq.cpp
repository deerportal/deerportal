#include "boarddiamondseq.h"

BoardDiamondSeq::BoardDiamondSeq(TextureHolder *textures)
{
    this->textures = textures;
    for (int i=0;  i<DP::diamondsNumber; i++)
    {
        diamonds[i] = BoardDiamond(this->textures,
                DP::DIAMONDS_SETUP[i][0],
                DP::DIAMONDS_SETUP[i][1],
                DP::DIAMONDS_SETUP[i][2]);
    }
    reorder();
}

void BoardDiamondSeq::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (int i=0;  i<DP::diamondsNumber; i++)
    {
        target.draw(diamonds[i],states);
    }
}

std::array<int,DP::diamondsNumber> BoardDiamondSeq::getBoardPositions()
{
    std::array<int, DP::diamondsNumber> results;
    for (int i=0;  i<DP::diamondsNumber; i++)
    {
        results[i] = diamonds[i].getBoardPosition();
    }
    return results;
}

//bool BoardDiamondSeq::ifFieldIsEmpty(int pos, int element)
//{
//    for (int i=element*DP::diamondsNumber/4;i<(DP::diamondsNumber/4)+(element*DP::diamondsNumber/4);i++)
//    {
//        if (diamonds[i].boardPosition==pos)
//            return false;
//    }
//    return true;

//}

bool BoardDiamondSeq::ifFieldIsEmpty(int pos)
{
    for (int element=0;element<4;element++)
    {
        for (int i=element*DP::diamondsNumber/4;i<(DP::diamondsNumber/4)+(element*DP::diamondsNumber/4);i++)
        {
            if (diamonds[i].boardPosition==pos)
               return false;
         }
    }
    return true;

}

int BoardDiamondSeq::getRandomPos(int playerNumber)
{
    std::set<int> setSteps(DP::occupiedFields[playerNumber].cbegin(), DP::occupiedFields[playerNumber].cend());
    for (int i=playerNumber*DP::diamondsNumber/4;i<(DP::diamondsNumber/4)+(playerNumber*DP::diamondsNumber/4);i++)
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
        for (int i=start*DP::diamondsNumber/4;i<(DP::diamondsNumber/4)+(start*DP::diamondsNumber/4);i++)
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

    for (int i=start*DP::diamondsNumber/4;i<(DP::diamondsNumber/4)+(start*DP::diamondsNumber/4);i++)
    {
        int step = getRandomPos(newElement);
        diamonds[i].setBoardPosition(step);
    }

}

void BoardDiamondSeq::collectField(int pos)
{
    for (int i=0;  i<DP::diamondsNumber; i++)
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
    for (int i=0;  i<DP::diamondsNumber; i++)
    {
        if (diamonds[i].boardPosition==pos)
        {
            return diamonds[i].idNumber;
        }
    }
    return -1;
}

