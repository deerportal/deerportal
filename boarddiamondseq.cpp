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

void BoardDiamondSeq::reorder()
{
    srand((unsigned)time(0));

    for (int element=0;element<4;element++)
    {std::cout << "UP reorder " << ""<< efc::diamondsNumber/4 << "" << element << std::endl;
        int start = element;
        for (int i=start*14;i<(efc::diamondsNumber/4)+(start*14);i++)
        {
            std::cout << "reorder " << i << std::endl;
            int step = rand()%efc::numberSteps;
            diamonds[i].setBoardPosition(efc::occupiedFields[start][step]);
        }
    }

}

    ;

