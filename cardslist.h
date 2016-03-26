#ifndef CARDSLIST_H
#define CARDSLIST_H
#include <vector>
#include "card.h"

class CardsList
{
public:
    CardsList();
    CardsList(int element);
    int amount;
    void addCard(Card card);
    std::vector<Card*> sd;
    int element;


};

#endif // CARDSLIST_H
