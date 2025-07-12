#include "cardslist.h"

CardsList::CardsList() : currentCard(0), element(0), invisibleLeft(0), active(true) {}

CardsList::CardsList(int element) : active(true) {
  //    amount = 32; //TODO: ???
  this->element = element;
  currentCard = 0;
  invisibleLeft = 0.0f; // Transparency for effect
}

void CardsList::shufflePile() {
  active = true;
  std::srand(std::time(0));
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  shuffle(cardsPile.begin(), cardsPile.end(), std::default_random_engine(seed));
}

// void CardsList::addCard(Card card)
//{

//}
