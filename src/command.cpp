#include <algorithm>
#include <iterator>
#include "game.h"
#include "command.h"
#include "data.h"

Command::Command(DP::Game &currentGame) :
    game(currentGame)
{

}

/*!
 * \brief Command::removeCard
 * \param playerNumber number of the player which cards will be removed
 * \return
 */
bool Command::removeCard(int playerNumber)
{
    /*! This should works as following:
     * - iterate over BoardDiamondSeq
     * - find diamonds belonging to the playerNumber
     * - create an array from the aboves
     * - get a random element
     * - remove from the board - set not active
     */

    std::vector<int> diamonds;
    for (auto&& i: game.boardDiamonds.diamonds)
    {
        int fieldNumber = i.playerNumber;
        if ((fieldNumber==playerNumber) && (i.boardPosition>-1)
                && (i.idNumber<4))
        {
            diamonds.push_back(i.boardPosition);
        }
    }
    int numberDiamonds = diamonds.size();
    if (numberDiamonds>0)
    {
        int elemToRemove = rand() %  numberDiamonds;
        game.boardDiamonds.collectField(diamonds[elemToRemove]);
        return true;
    }
    return false;
}

void Command::removeAllCardElement(int elementNumber) const
{
    /*! This should works as following:
     * - iterate over BoardDiamondSeq
     * - find diamonds belonging to the playerNumber
     * - create an array from the aboves
     * - get a random element
     * - remove from the board - set not active
     */

    for (auto&& i: game.boardDiamonds.diamonds)
    {
        int cardElementNumber = i.idNumber;
        if ((i.boardPosition>-1) && (cardElementNumber==elementNumber))
        {
            i.boardPosition=-1;
        };
    }

}
/*!
 * \brief Command::removeDiamond
 * \param playerNumber
 * \return if diamond had been removed
 */
bool Command::removeDiamond(int playerNumber)
{
    /*! This should works as following:
     * - iterate over BoardDiamondSeq
     * - find diamonds belonging to the playerNumber
     * - create an array from the aboves
     * - get a random element
     * - remove from the board - set not active
     */

    std::vector<int> diamonds;
    for (auto&& i: game.boardDiamonds.diamonds)
    {
        int fieldNumber = i.playerNumber;
        if ((fieldNumber==playerNumber) && (i.boardPosition>-1)
                && (i.idNumber==4))
        {
            diamonds.push_back(i.boardPosition);
        }
    }
    int numberDiamonds = diamonds.size();
    if (numberDiamonds>0)
    {
        int elemToRemove = rand() %  numberDiamonds;
        game.boardDiamonds.collectField(diamonds[elemToRemove]);
        return true;
    }
    return false;
}

void Command::freezePlayer(int playerNumber)
{
    game.players[playerNumber].frozenLeft += 1;
}

 void Command::removeAllItems(int playerNumber) const
{
    for (auto&& i: game.boardDiamonds.diamonds)
    {
        int itemPlayerNumber = i.playerNumber;
        if ((i.boardPosition>-1) && (itemPlayerNumber==playerNumber))
        {
            i.boardPosition=-1;
        };
    }
}

/*!
 * \brief Command::processField when the player enters the field
 * \param pos
 */
void Command::processField(int pos)
{
    bool startField = std::find(std::begin(DP::startPlayers),
                            std::end(DP::startPlayers), pos)
            != std::end(DP::startPlayers);

    if ((startField) && (DP::startPlayers[game.turn]==pos))
    {
        game.banner.setText("meditation");
        game.boardDiamonds.reorder(game.turn);
        game.sfx.soundMeditation.play();
    }

    if (game.boardDiamonds.ifFieldIsEmpty(pos)==false)
    {
        game.sfx.playCollect();
        if (game.boardDiamonds.getNumberForField(pos)==4)
        {
            game.players[game.turn].cash += 1;

        } else if (game.boardDiamonds.getNumberForField(pos)<4)
        {
            processCard(pos);
        }

        game.boardDiamonds.collectField(pos);
    }
}

/*!
 * \brief Command::processCard when the user enters the field
 * \param pos
 */
void Command::processCard(int pos)
{
    int tokenNumber = game.boardDiamonds.getNumberForField(pos);
    std::string cardType = game.cardsDeck.getTitle(tokenNumber);
    if (tokenNumber!=game.turn)
    {
        if (cardType == "diamond")
        {
            removeDiamond(game.boardDiamonds.getNumberForField(pos));
            game.players[game.turn].cash += 1;

        } else if (cardType == "stop")
        {
            freezePlayer(tokenNumber);
        } else if (cardType == "card")
        {
            removeCard(game.boardDiamonds.getNumberForField(pos));

        } else if (cardType == "diamond x 2")
        {
            if (removeDiamond(game.boardDiamonds.getNumberForField(pos)))
                game.players[game.turn].cash += 1;
            if (removeDiamond(game.boardDiamonds.getNumberForField(pos)))
                game.players[game.turn].cash += 1;

        }
    }
    game.cardsDeck.nextCard(tokenNumber);
    game.sfx.playCard();
}

//int Command::processGui(sf::Vector2f posGui)
//{
//    return posGui.x + posGui.y; /*! TODO: add processing some actions */
//}
