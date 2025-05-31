#include "game-core.h"
#include "game.h"
#include "calendar.h"
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace DP {

GameCore::GameCore(Game* gameInstance)
    : game(gameInstance)
{
}

GameCore::~GameCore()
{
}

void GameCore::initBoard()
{
    time_t t = time(0);
    struct tm * now = localtime( & t );
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    game->paganHolidayString = getHoliday(month, day);
    game->paganHolidayTxt->setString(game->paganHolidayString);

    game->sfxClick.setBuffer(game->sfxClickBuffer);
    game->sfxDone.setBuffer(game->sfxDoneBuffer);
    game->spriteBackgroundDark->setTexture(game->textures.backgroundDark);
    game->spriteBackgroundDark->setPosition(sf::Vector2f(0,0));
    game->spriteLestBegin->setTexture(game->textures.textureLetsBegin);
    game->viewTiles.setViewport(sf::FloatRect({0.15f,0.1f}, {1.0f, 1.0f}));
    game->viewGui.setViewport(sf::FloatRect({0.806f,0.066f}, {1, 1}));

    game->groupHud.setFont(&game->gameFont);
    game->groupHud.setSeason(game->currentSeason);
    game->groupHud.setRoundName(game->roundNumber);

    game->cardsDeck.setFonts(&game->gameFont);
    game->spriteBigDiamond->setTexture(game->textures.textureBigDiamond);
    game->spriteBigDiamond->setPosition(sf::Vector2f(474,342));  // Original 0.8.2 position coordinates
    game->spriteBigDiamond->setColor(sf::Color (255, 255, 255,196));
    restartGame();
    launchNextPlayer();

    game->endGameTxt->setFont(game->gameFont);
    game->endGameTxt->setString("End of the Game");
    game->endGameTxt->setCharacterSize(30);

    sf::FloatRect ss = game->endGameTxt->getLocalBounds();
    game->endGameTxt->setPosition(sf::Vector2f((1360/2)-(ss.size.x/2),60));

    game->bubble.setPosition(game->players[game->turn].characters[0].getPosition().x-30,
            game->players[game->turn].characters[0].getPosition().y-45);

    game->txtSurvivorsLabel->setString("Survivors");
    game->txtSurvivorsLabel->setFont(game->gameFont);
    game->txtSurvivorsLabel->setCharacterSize(30);
    sf::FloatRect rectSurvivors = game->txtSurvivorsLabel->getLocalBounds();
    game->txtSurvivorsLabel->setPosition(sf::Vector2f((1360/2)-(rectSurvivors.size.x/2),200));

    game->txtLoosersLabel->setString("Digested by The Elements");
    game->txtLoosersLabel->setFont(game->gameFont);
    game->txtLoosersLabel->setCharacterSize(30);
    sf::FloatRect rectLoosers = game->txtLoosersLabel->getLocalBounds();
    game->txtLoosersLabel->setPosition(sf::Vector2f((1360/2)-(rectLoosers.size.x/2),500));
    game->credits.setTxt(0);
}

void GameCore::restartGame()
{
    Player playerHud1(&game->textures, &game->gameFont,0);
    Player playerHud2(&game->textures, &game->gameFont,1);
    Player playerHud3(&game->textures, &game->gameFont,2);
    Player playerHud4(&game->textures, &game->gameFont,3);
    game->players[0] = std::move(playerHud1);
    game->players[1] = std::move(playerHud2);
    game->players[3] = std::move(playerHud3);
    game->players[2] = std::move(playerHud4);
    game->players[0].setActive(true);
    setCurrentNeighbours();
    game->diceResultPlayer = 6;
    game->players[game->turn].characters[0].diceResult = game->diceResultPlayer;
    game->roundDice.setColor(game->turn);

    for (int i=0;i<4;i++)
    {
        game->players[i].restartPlayer();
        game->players[i].reachedPortal = false;

        game->boardDiamonds.reorder(i);
        game->bubble.setPosition(game->players[i].characters[0].getPosition().x-30,
                game->players[i].characters[0].getPosition().y-45);
        game->players[i].done=false;
    }
    game->numberFinishedPlayers = 0;
    game->turn = 0;
    game->currentSeason = 1;
    game->roundNumber = 1;
    game->month = 0;
    game->cardsDeck.reloadCards();
    game->deerModeActive = false;
    game->deerModeCounter = 16;
    game->bigDiamondActive = true;  // Ensure big diamond is visible on game restart
}

void GameCore::setCurrentNeighbours()
{
    game->currentNeighbours = game->players[game->turn].getNeighbours();
}

void GameCore::endGame()
{
    game->musicGame.stop();
    game->currentState = Game::state_end_game;
    game->downTimeCounter = 0;
    game->numberFinishedPlayers = 4;
}

void GameCore::throwDiceMove()
{
    // Throw a dice action
    game->diceResultPlayer = game->roundDice.throwDiceSix();
    game->players[game->turn].characters[0].diceResult = game->diceResultPlayer;
    game->currentState = Game::state_game;
    game->bubble.state = BubbleState::MOVE;
    game->nextRotateElem.reset();
    game->prevRotateElem.reset();
}

void GameCore::playerMakeMove(int mousePos)
{
    game->players[game->turn].setFigurePos(mousePos);
    game->commandManager.processField(mousePos);
    const int *possibleExit = std::find(std::begin(DP::endPlayers),
                                        std::end(DP::endPlayers), mousePos);
    if (possibleExit != DP::endPlayers+4) {
        game->players[game->turn].done=true;
        game->players[game->turn].reachedPortal=true;
        game->commandManager.removeAllItems(game->turn);
        if (game->numberFinishedPlayers == 0)
        {
            game->players[game->turn].reachedPortalFirst=true;
            int turnover =  (rand() %  2)+5;

            game->players[game->turn].cash += turnover;
            game->players[game->turn].updatePlayer();
            startDeerMode();
        }

        game->sfx.soundPortal.play();
        game->numberFinishedPlayers += 1;
        if (game->numberFinishedPlayers > 3)
        {
            endGame();
            return;
        }
    }
    nextPlayer();
    return;
}

int GameCore::mostDiamonds() const
{
    std::array<int,4> results = {{game->players[0].cash,game->players[1].cash,game->players[2].cash,game->players[3].cash}};
    auto minmax = std::minmax_element(std::begin(results), std::end(results));
    int maxResult = *(minmax.second);
    int result = 0;
    int pos = -1;
    for (int i=0; i<4;i++)
    {
        if (game->players[i].cash == maxResult)
        {
            result += 1;
            pos = i;
        }
    };
    if (result==1)
    {
        return pos;
    }
    return -1;
}

void GameCore::nextPlayer()
{
    // End of game - we don't calculate more players
    if (game->currentState == Game::state_end_game)
    {
        return;
    }

    // End of game - we don't calculate more players
    if (game->numberFinishedPlayers == 4)
    {
        endGame();
        return ;
    }

    // Update old player
    game->players[game->turn].updatePlayer();
    if (game->turn > 2)
    {
        nextRound();
        return;
    }
    game->turn++;
    launchNextPlayer();
}

void GameCore::nextRound()
{
    game->turn = 0;
    game->roundNumber += 1;
    game->month++;
    if (game->month == 13)
        game->month = 1;
    if (game->month % 4 == 0)
        game->currentSeason++;
    if (game->currentSeason > 3)
        game->currentSeason = 0;

    launchNextPlayer();
}

void GameCore::launchNextPlayer()
{
    if (game->deerModeActive)
    {
        game->deerModeCounter -= 1;
    }

    if (game->deerModeCounter < 0)
    {
        endGame();
        return ;
    }
    // Just control
    if (game->players[game->turn].done == true)
    {
        nextPlayer();
        return;
    }

    // Frozen player
    if (game->players[game->turn].frozenLeft > 0)
    {
        game->players[game->turn].frozenLeft -= 1;
        nextPlayer();
        return;
    }

    // Here we process new player
    game->selector.changeColor(game->turn);
    for (int i=0;i<4;i++)
    {
        if (i == game->turn)
        {
            game->players[i].setActive(true);
            game->currentNeighbours = game->players[i].getNeighbours();
        }
        else
            game->players[i].setActive(false);
    }

    game->sfxClick.play();
    game->diceResultPlayer = 6;
    game->roundDice.setDiceTexture(game->diceResultPlayer);

    game->players[game->turn].characters[0].diceResult = game->diceResultPlayer;
    game->groupHud.setRoundName(game->roundNumber);
    if (!game->deerModeActive) {
        if (mostDiamonds() == game->turn) {
            game->players[game->turn].reachPortalMode = true;
            game->bigDiamondActive = true;   // Show diamond for player with most diamonds
        } else {
            game->players[game->turn].reachPortalMode = false;
            // bigDiamondActive remains unchanged here, allowing it to persist through ties if previously active
        }
    } else {
        // In Deer Mode: bigDiamond is already false (set by startDeerMode)
        // Ensure players cannot enter portal mode through the 'most diamonds' mechanic here.
        game->players[game->turn].reachPortalMode = false;
    }

    if (game->deerModeActive == false)
    {
        game->groupHud.setSeason(game->currentSeason);
    } else
    {
        game->groupHud.setDeerModeActive();
        int number = (game->deerModeCounter/4);

        if (game->deerModeCounter < 16)
            number += 1;
        if (number < 0)
        {
            endGame();
            return;
        }
        game->groupHud.setDeerModeCounter(number);
    }
    game->currentState = Game::state_roll_dice;
    game->bubble.state = BubbleState::DICE;
    game->roundDice.setColor(game->turn);
    game->roundDice.setFaces(0);  // Set dice to show "waiting to click" face (face 0)
    game->bubble.setPosition(game->players[game->turn].characters[0].getPosition().x-30,
            game->players[game->turn].characters[0].getPosition().y-45);

    game->cpuTimeThinking = game->cpuTimeThinkingInterval;
}

void GameCore::startDeerMode()
{
    game->deerModeActive = true;
    game->deerModeCounter = 16;
    game->banner.setText("deer mode");
    game->bigDiamondActive = false;
    game->sfx.soundDeerMode.play();
}

void GameCore::command(const std::string& command)
{
    if (command.compare("end_of_round") == 0)
    {
        std::string subResult = command.substr(13);
        game->guiRoundDice.active = true;
        game->guiRoundDice.setTitle(subResult);
        game->currentState = Game::state_gui_end_round;
    }

    if (command == "end_turn")
        nextPlayer();
}

void GameCore::update(const sf::Time& frameTime)
{
    updatePlayerTimers(frameTime);
    updateVisualEffects(frameTime);
    updateAIBehavior(frameTime);
    updateGameClock(frameTime);
}

void GameCore::updatePlayerTimers(const sf::Time& frameTime)
{
    game->cpuTimeThinking -= frameTime.asSeconds();
}

void GameCore::updateVisualEffects(const sf::Time& frameTime)
{
    if (game->oscilatorInc)
    {
        game->oscilator += frameTime.asSeconds();
    } else {
        game->oscilator -= frameTime.asSeconds();
    }

    if (game->oscilator < -1)
        game->oscilatorInc = true;

    if (game->oscilator > 1)
        game->oscilatorInc = false;

    float modifier = sin(game->oscilator/2.5)*30.0f;
    game->spriteBigDiamond->setPosition(sf::Vector2f(474,342+modifier));
}

void GameCore::updateAIBehavior(const sf::Time& frameTime)
{
    if (game->currentState == Game::state_roll_dice)
    {
        if ((game->cpuTimeThinking < 0) && (game->players[game->turn].human == false))
        {
            game->cpuTimeThinking = game->cpuTimeThinkingInterval;
            throwDiceMove();
        }
    }

    if (game->currentState == Game::state_game)
    {
        std::array<int,2> currentMovements = game->players[game->turn].getMovements(game->diceResultPlayer);

        if ((game->cpuTimeThinking < 0) &&  (game->players[game->turn].human == false))
        {
            std::vector<int> listRandomPos;

            if (currentMovements[0] > -1) {
                listRandomPos.push_back(currentMovements[0]);
            }
            if (currentMovements[1] > -1) {
                listRandomPos.push_back(currentMovements[1]);
            }

            unsigned int sizeRndPos = listRandomPos.size();
            if (sizeRndPos == 0) {

            } else if  (sizeRndPos == 1) {
                playerMakeMove(listRandomPos[0]);

            } else if (sizeRndPos == 2) {

                if (game->deerModeActive)
                {
                    playerMakeMove(listRandomPos[1]);
                } else
                {
                    if (game->players[game->turn].reachPortalMode == true)
                    {
                        playerMakeMove(listRandomPos[1]);
                    }
                    else
                    {
                        if (game->boardDiamonds.ifFieldIsEmpty(listRandomPos[1]) == false)
                        {
                            playerMakeMove(listRandomPos[1]);
                            return;
                        }
                        if (game->boardDiamonds.ifFieldIsEmpty(listRandomPos[0]) == false)
                        {
                            playerMakeMove(listRandomPos[0]);
                            return;
                        }
                        if ((game->boardDiamonds.ifFieldIsEmpty(listRandomPos[0]) == false) && 
                            (game->boardDiamonds.ifFieldIsEmpty(listRandomPos[1]) == false))
                        {
                            playerMakeMove(listRandomPos[1]);
                            return;
                        }
                        int randPos = rand() % 2;
                        playerMakeMove(listRandomPos[randPos]);
                    };
                }
            }
        }

        if (currentMovements[0] > -1)
        {
            game->prevRotateElem.spriteRotate->setPosition(game->players[game->turn].characters[0].leftChar->getPosition());
            game->prevRotateElem.spriteRotate->move(sf::Vector2f(10,20));

            // Modificator to fit on the bigger view
            game->prevRotateElem.spriteRotate->move(sf::Vector2f(-202,-76));

            game->prevRotateElem.update(frameTime);
            game->prevRotateElem.setColor();
        }
        if (currentMovements[1] > -1)
        {
            game->nextRotateElem.spriteRotate->setPosition(game->players[game->turn].characters[0].rightChar->getPosition());
            game->nextRotateElem.spriteRotate->move(sf::Vector2f(10,20));

            // Modificator to fit on the bigger view
            game->nextRotateElem.spriteRotate->move(sf::Vector2f(-202,-76));
            game->nextRotateElem.update(frameTime);
            game->nextRotateElem.setColor();
        }
    }
}

void GameCore::updateGameClock(const sf::Time& frameTime)
{
    if (game->currentState == Game::state_lets_begin)
    {
        game->downTimeCounter += frameTime.asSeconds();
        game->spriteLestBegin->setColor(sf::Color(255,255,255,255-(game->downTimeCounter*35)));
        if (game->downTimeCounter > 5)
        {
            game->currentState = Game::state_roll_dice;
            game->bubble.state = BubbleState::DICE;
        }
    }

    if (game->currentState == Game::state_end_game)
    {
        game->downTimeCounter += frameTime.asSeconds();
    }
}

// State query methods
bool GameCore::isGameEnded() const
{
    return game->currentState == Game::state_end_game;
}

int GameCore::getCurrentPlayer() const
{
    return game->turn;
}

bool GameCore::isDeerModeActive() const
{
    return game->deerModeActive;
}

void GameCore::checkEndGameConditions()
{
    if (game->numberFinishedPlayers >= 4)
    {
        endGame();
    }
}

void GameCore::processPlayerTurn()
{
    // Helper method for complex turn processing logic
    // Can be expanded as needed
}

void GameCore::processGameStateUpdates(const sf::Time& frameTime)
{
    game->cardsDeck.update(frameTime);
    for (int i=0;i<4;i++)
    {
        game->players[i].play();
        game->players[i].update(frameTime);
    }

    game->bubble.update(frameTime);
}

void GameCore::handlePlayerMovement(const sf::Time& frameTime)
{
    // Movement-specific updates handled in updateAIBehavior
    // This method can be expanded for more complex movement logic
}

} // namespace DP 