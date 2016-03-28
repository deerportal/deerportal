#ifndef CARD_H
#define CARD_H
#include <string>

class Card
{
public:
    Card();
    int id;
    int elementNumber;
    int amount;
    std::string cardType;
};

#endif // CARD_H
