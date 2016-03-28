#ifndef CARDSLIST_H
#define CARDSLIST_H
#include <vector>
#include <array>
#include "card.h"

namespace efc {
    const static int PILE_SIZE = 3;
}

class CardsList
{
public:
    CardsList(); // To create an array of CardsList
    CardsList(int element);
    int amount;

//    std::vector<Card*> sd;
    /*!
     * \brief element Number of the element
     */

    std::array<Card, efc::PILE_SIZE> cardsPile;
    int currentCard;
    int element; /*!< Number of the element */


    float invisibleLeft;



};

#endif // CARDSLIST_H
