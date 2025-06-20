#include "game-input.h"
#include "game.h"
#include "tilemap.h"  // For TILE_SIZE, BOARD_SIZE, transCords

namespace DP {

GameInput::GameInput(Game* gameInstance)
    : game(gameInstance)
    , mouseOverBoard(false)
    , currentInputMode(0)
{
}

GameInput::~GameInput()
{
}

void GameInput::processEvents(sf::RenderWindow& window)
{
    while (const std::optional<sf::Event> eventOpt = window.pollEvent())
    {
        const sf::Event& event = *eventOpt;
        
        if (event.is<sf::Event::Closed>())
        {
            window.close();
            return;
        }
        
        handleKeyboardInput(event);
        handleMouseInput(event);
    }
}

void GameInput::handleKeyboardInput(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Escape)
        {
            game->window.close();
        }
        
        // Additional keyboard handling can be added here
        // Original particle system controls were commented out in the original code
    }
}

void GameInput::handleMouseInput(const sf::Event& event)
{
    // Get mouse position and convert to different coordinate systems
    sf::Vector2i localPositionTmp = sf::Mouse::getPosition(game->window);
    sf::Vector2f localPosition = game->window.mapPixelToCoords(localPositionTmp, game->viewTiles);
    sf::Vector2f localPositionFull = game->window.mapPixelToCoords(localPositionTmp, game->viewFull);
    
    int mousePosX = (int)localPosition.x / DP::TILE_SIZE;
    int mousePosY = (int)localPosition.y / DP::TILE_SIZE;
    int mousePos = DP::transCords(sf::Vector2i(mousePosX, mousePosY));
    
    // Update selector position and board hover state
    updateSelector(localPosition);
    
    // Handle mouse click events
    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left)
        {
            handleLeftClick(localPosition, localPositionFull, mousePos);
        }
    }
}

void GameInput::updateSelector(sf::Vector2f localPosition)
{
    // Update mouse hover state for game board
    if (game->currentState == Game::state_game)
    {
        if ((localPosition.x > DP::TILE_SIZE * DP::BOARD_SIZE) || 
            (localPosition.x < 0) || 
            (localPosition.y > DP::TILE_SIZE * DP::BOARD_SIZE) || 
            (localPosition.y < 0))
        {
            game->showPlayerBoardElems = false;
        } 
        else 
        {
            game->showPlayerBoardElems = true;
        }
    }

    // Update selector position if within valid board bounds
    if ((localPosition.x >= 0) && (localPosition.y >= 0) && 
        (localPosition.x <= DP::BOARD_SIZE * DP::TILE_SIZE) && 
        (localPosition.y <= DP::BOARD_SIZE * DP::TILE_SIZE))
    {
        game->selector.setPosition(sf::Vector2f(
            (int)(localPosition.x / DP::TILE_SIZE) * DP::TILE_SIZE, 
            (int)(localPosition.y / DP::TILE_SIZE) * DP::TILE_SIZE
        ));
    }
}

void GameInput::handleLeftClick(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    switch (game->currentState)
    {
        case Game::state_game:
            processGameInput(pos, posFull, mousePos);
            break;
            
        case Game::state_setup_players:
            processSetupInput(pos, posFull, mousePos);
            break;
            
        case Game::state_roll_dice:
            processRollDiceInput(pos, posFull, mousePos);
            break;
            
        case Game::state_menu:
            processMenuInput(pos, posFull, mousePos);
            break;
            
        case Game::state_gui_end_round:
            processGuiEndRoundInput(pos, posFull, mousePos);
            break;
            
        case Game::state_lets_begin:
            processLetsBeginInput(pos, posFull, mousePos);
            break;
            
        case Game::state_end_game:
            processEndGameInput(pos, posFull, mousePos);
            break;
            
        default:
            // Handle other states if needed
            break;
    }
}

void GameInput::processGameInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    if (game->players[game->turn].human)
    {
        std::array<int,2> movements = game->players[game->turn].getMovements(game->diceResultPlayer);
        if ((mousePos == movements[0]) || (mousePos == movements[1]))
        {
            game->playerMakeMove(mousePos);
        }
    }
}

void GameInput::processSetupInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    // Check for clicks on player AI/Human toggle buttons
    for (int i = 0; i < 4; i++)
    {
        sf::FloatRect spriteHumanRect(game->players[i].spriteAI->getGlobalBounds());
        if (spriteHumanRect.contains(posFull))
        {
            game->players[i].swapHuman();
        }
    }
    
    // Check for start game button click
    sf::IntRect startGameRect({580, 640}, {180, 80});
    if (startGameRect.contains(sf::Vector2i((int)posFull.x, (int)posFull.y)))
    {
        game->bigDiamondActive = true;
        game->banner.setText("start game");
        game->currentState = Game::state_roll_dice;
        game->launchNextPlayer();
    }
}

void GameInput::processRollDiceInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    if (game->players[game->turn].human)
    {
        sf::FloatRect diceRect(game->roundDice.spriteDice->getGlobalBounds());
        if (diceRect.contains(posFull))
        {
            game->throwDiceMove();
        }
    }
}

void GameInput::processMenuInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    game->downTimeCounter = 0;
    game->stateManager->hideMenu();
    game->stateManager->showGameBoard();
}

void GameInput::processGuiEndRoundInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    std::string resultCommand = game->guiRoundDice.getElem(pos);
    game->command(resultCommand);
}

void GameInput::processLetsBeginInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    if (game->downTimeCounter > 1)
    {
        game->currentState = Game::state_roll_dice;
        game->restartGame();
        game->launchNextPlayer();
    }
}

void GameInput::processEndGameInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos)
{
    if (game->downTimeCounter > 2)
    {
        game->currentState = Game::state_menu;
        game->musicMenu.play();
        game->restartGame();
    }
}

sf::Vector2f GameInput::getMousePosition(sf::RenderWindow& window) const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mousePos, game->viewFull);
}

sf::Vector2f GameInput::mapToTileCoords(sf::Vector2f screenPos, sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mousePos, game->viewTiles);
}

sf::Vector2f GameInput::mapToGuiCoords(sf::Vector2f screenPos, sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mousePos, game->viewGui);
}

int GameInput::convertToTileIndex(sf::Vector2f tileCoords)
{
    int mousePosX = (int)tileCoords.x / DP::TILE_SIZE;
    int mousePosY = (int)tileCoords.y / DP::TILE_SIZE;
    return DP::transCords(sf::Vector2i(mousePosX, mousePosY));
}

bool GameInput::isInputAllowed() const
{
    // Add logic to determine if input should be processed based on current game state
    return true;  // For now, always allow input
}

void GameInput::setInputMode(int gameState)
{
    currentInputMode = gameState;
}

void GameInput::validateMousePosition(sf::Vector2f position)
{
    // Add validation logic if needed
    lastMousePosition = position;
}

} // namespace DP 