#ifndef CARD_H
#define CARD_H
#include <string>

class Card
{
public:
    Card();
    int id;
    int elementNumber; //location
    int amount;
    std::string cardType;
    int cardTypeInt;
};

#endif // CARD_H
