#ifndef CARD_H
#define CARD_H
#include <array>
#include <string>

namespace DP {
const static std::array<std::string, 4> cardsTypes = {{"stop", "card", "diamond", "diamond x 2"}};

const static std::array<std::array<std::string, 4>, 4> cardsTitles = {
    {{{"stop", "card", "diamond", "diamond x 2"}},
     {{"stop", "card", "diamond", "diamond x 2"}},
     {{"stop", "card", "diamond", "diamond x 2"}},
     {{"stop", "card", "diamond", "diamond x 2"}}}

};
} // namespace DP
/*!
 * \brief The Card definition
 */
class Card {
public:
  Card();
  int id;
  int elementNumber; // location
  int amount;

  std::string cardType;

  // Card type from the cardsTypes
  int cardTypeInt;
  // Title of the card, will be presented at the bottom of the card
  std::string cardTitle;
};

#endif // CARD_H
