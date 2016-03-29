#include "cardslist.h"


CardsList::CardsList():
    currentCard(0),
    invisibleLeft(0)
{

}

CardsList::CardsList(int element)
{
    amount = 32;
    this->element = element;
    currentCard = 0;
    invisibleLeft = 0.0f;

}

void CardsList::shufflePile()
{
    std::srand(std::time(0));
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (cardsPile.begin(), cardsPile.end(), std::default_random_engine(seed));
}


//void CardsList::addCard(Card card)
//{

//}
