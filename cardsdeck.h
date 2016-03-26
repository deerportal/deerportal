#ifndef CARDSDECK_H
#define CARDSDECK_H
#include <array>
#include "cardslist.h"

class CardsDeck
{
public:
    CardsDeck();
    std::array<CardsList, 4> cardsList;
};

#endif // CARDSDECK_H
