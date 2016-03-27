#ifndef CARDSLIST_H
#define CARDSLIST_H
#include <vector>
#include "card.h"

class CardsList
{
public:
    CardsList(); // To create an array of CardsList
    CardsList(int element);
    int amount;
    void addCard(Card card);
    std::vector<Card*> sd;
    /*!
     * \brief element Number of the element
     */
    int element; /*!< Number of the element */

    int var;     /*!< Detailed description after the member */
};

#endif // CARDSLIST_H
