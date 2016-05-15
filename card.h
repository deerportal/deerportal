#ifndef CARD_H
#define CARD_H
#include <string>
#include <array>

namespace efc {
const static std::array<std::string, 4> cardsTypes = {
    "stop", "card", "diamond", "diamond x 2"
};

}
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
