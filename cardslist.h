#ifndef CARDSLIST_H
#define CARDSLIST_H
#include <vector>
#include <array>
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "card.h"

namespace efc {
    const static int PILE_SIZE = 4;
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
    void shufflePile();


};

#endif // CARDSLIST_H
