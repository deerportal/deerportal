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


//void CardsList::addCard(Card card)
//{

//}
