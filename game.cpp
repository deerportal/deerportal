#include "game.h"
#include "particle.h"
#include "calendar.h"


namespace efc {

int initScreenX = 1360;
int initScreenY = 768;
int currentSeason = 1;
int month = 0;

/*!
 * \brief The ResultTable struct contains data for the end of the game screen.
 */
struct ResultTable
{
    int playerNumber;
    int playerResult;
    bool reachedPortal;
    bool reachedPortalFirst;
    ResultTable(int number,int result, bool portal, bool portalFirst) :
        playerNumber(number), playerResult(result), reachedPortal(portal), reachedPortalFirst(portalFirst) {}
    bool operator < (const ResultTable& otherResult) const
    {
        if ((playerResult!=otherResult.playerResult) || ((reachedPortal==false) && (otherResult.reachedPortal==false)))
            return (playerResult > otherResult.playerResult);
        else if (reachedPortalFirst==true){
            return false;

        } else
        {
            return true;
        }

        ;
    }
};
/*!
 * \brief Game::setTxtEndGameAmount orders the results text for the end of the game.
 */
void Game::setTxtEndGameAmount(){
    std::string elementNames[4] = {"Water","Earth", "Fire", "Air"};

    int width=1360;
    //    int height = 768;
    int startHeight = 100;
    int separator = 40;
    std::array<ResultTable, 4> results = {
        {
            ResultTable(0,players[0].cash, players[0].reachedPortal,players[0].reachedPortalFirst),
            ResultTable(1,players[1].cash, players[0].reachedPortal,players[0].reachedPortalFirst),
            ResultTable(2,players[2].cash, players[0].reachedPortal,players[0].reachedPortalFirst),
            ResultTable(3,players[3].cash, players[0].reachedPortal,players[0].reachedPortalFirst)
        }
    };

    std::vector < ResultTable > resultsVector;
    txtSurvivors.clear();
    txtLoosers.clear();

    for (int i=0;i<4;i++)
    {
        resultsVector.push_back(results[i]);

    };



    std::sort(resultsVector.begin(), resultsVector.end());
    txtWinner.setFont(gameFont);
    txtWinner.setCharacterSize(40);
    for (int i=0;i<4;i++)
    {
        int playerNumber = resultsVector[i].playerNumber;
        std::string elementName = elementNames[playerNumber];
        std::string label = elementName+ " " + std::to_string(players[playerNumber].cash);
        endGameTxtAmount[i].setString(label);
        sf::FloatRect ss = endGameTxtAmount[i].getLocalBounds();
        endGameTxtAmount[i].setPosition((width/2)-(ss.width/2),separator+(i*separator)+startHeight);

        sf::Text tmpText;
        tmpText.setFont(gameFont);
        tmpText.setCharacterSize(25);
        tmpText.setString(elementName+ " " + std::to_string(players[playerNumber].cash));


        sf::FloatRect rectTxt = tmpText.getLocalBounds();



        if (results[i].reachedPortal==true)
        {
            tmpText.setPosition((1360/2)-(rectTxt.width/2),240+(i*separator));
            txtSurvivors.push_back(tmpText);
        } else
        {
            tmpText.setPosition((1360/2)-(rectTxt.width/2),540+(i*separator));
            txtLoosers.push_back(tmpText);
        }

    }




}

void Game::initBoard()
{

    time_t t = time(0);
    struct tm * now = localtime( & t );
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    paganHolidayString =  getHoliday(month, day);
    //    std::cout << "HOLIDAY"<< paganHolidayString << std::endl;
    paganHolidayTxt.setString(paganHolidayString);

    sfxClick.setBuffer(sfxClickBuffer);
    sfxDone.setBuffer(sfxDoneBuffer);
    spriteBackgroundDark.setTexture(textures.backgroundDark);
    spriteBackgroundDark.setPosition(0,0);
    spriteLestBegin.setTexture(textures.textureLetsBegin);
    viewTiles.setViewport(sf::FloatRect(0.15f,0.1f, 1.0f, 1.0f));
    viewGui.setViewport(sf::FloatRect(0.806f,0.066f, 1, 1));

    groupHud.setFont(&gameFont);
    groupHud.setSeason(currentSeason);
    groupHud.setRoundName(roundNumber);

    cardsDeck.setFonts(&gameFont);
    restartGame();


    for (int i=0;i<4;i++)
    {
        endGameTxtAmount[i].setFont(gameFont);
        endGameTxtAmount[i].setCharacterSize(25);


    }

    endGameTxt.setFont(gameFont);
    endGameTxt.setString("Game Over");
    endGameTxt.setCharacterSize(30);


    sf::FloatRect ss = endGameTxt.getLocalBounds();



    endGameTxt.setPosition((1360/2)-(ss.width/2),0);

    setTxtEndGameAmount();
    bubble.setPosition(players[turn].characters[0].getPosition().x-30,
            players[turn].characters[0].getPosition().y-45);
    //    endGameTxt.set
    //    endGameTxt.setScale(2,2);

}

/*!
 * \brief Game::restartGame sets all the variables to the default value
 */
void Game::restartGame()
{

    PlayerHud playerHud1(&textures, &gameFont,0);
    PlayerHud playerHud2(&textures, &gameFont,1);
    PlayerHud playerHud3(&textures, &gameFont,2);
    PlayerHud playerHud4(&textures, &gameFont,3);
    players[0] = playerHud1;
    players[1] = playerHud2;
    players[3] = playerHud3;
    players[2] = playerHud4;
    players[0].setActive(true);
    setCurrentNeighbours();
    diceResultPlayer =  6;
    players[turn].characters[0].diceResult = diceResultPlayer;
    roundDice.setColor(turn);

    for (int i=0;i<4;i++)
    {
        players[i].restartPlayer();
        players[i].reachedPortal = false;

        boardDiamonds.reorder(i);
        bubble.setPosition(players[i].characters[0].getPosition().x-30,
                players[i].characters[0].getPosition().y-45);
        players[i].done=false;

    }
    numberFinishedPlayers = 0;
    turn = 0;
    currentSeason = 1;
    month = 0;
    cardsDeck.reloadCards();
    deerModeActive = false;
    deerModeCounter = 16;
    launchNextPlayer();

}

void Game::setCurrentNeighbours ()
{
    currentNeighbours = players[turn].getNeighbours();
}

/*!
 * \brief Game::loadAssets
 */
void Game::loadAssets()
{

    if (!gameFont.loadFromFile("assets/fnt/metal-mania.regular.ttf"))
    {
        std::exit(1);
    }
    if (!menuFont.loadFromFile("assets/fnt/metal-macabre.regular.ttf"))
    {
        std::exit(1);
    }



    if (!shaderBlur.loadFromFile("assets/shaders/blur.frag", sf::Shader::Fragment))
        std::exit(1);

    if (!shaderPixel.loadFromFile("assets/shaders/pixelate.frag", sf::Shader::Fragment))
        std::exit(1);
    if (!shaderDark.loadFromFile("assets/shaders/dark.frag", sf::Shader::Fragment))
        std::exit(1);

    if (!textureBackgroundArt.loadFromFile("assets/img/background_land.png"))
        std::exit(1);

    if (!musicGame.openFromFile("assets/audio/game.ogg"))
        std::exit(1);
    //    if (!musicBackground.openFromFile("assets/audio/wind2.ogg"))
    //        std::exit(1);
    if (!musicMenu.openFromFile("assets/audio/menu.ogg"))
        std::exit(1);


    if (!sfxClickBuffer.loadFromFile("assets/audio/click.ogg"))
        std::exit(1);

    if (!sfxDoneBuffer.loadFromFile("assets/audio/done.ogg"))
        std::exit(1);
    //    if (!textureBackground.loadFromFile("assets/img/background.png"))
    //        std::exit(1);

    spriteBackgroundArt.setTexture(textureBackgroundArt);
    menuTxt.setFont(gameFont);
    menuTxt.setCharacterSize(60);
    menuTxt.setString(gameTitle);
    int width = menuTxt.getLocalBounds().width;
    int height = menuTxt.getLocalBounds().height;
    menuTxt.setPosition(1050-(width/2),750-(height/2)-150);
    menuTxt.setColor(sf::Color(255, 255, 255, 85));
    cardsDeck.setFonts(&gameFont);

    paganHolidayTxt.setFont(gameFont);
    paganHolidayTxt.setCharacterSize(20);
    paganHolidayTxt.setPosition(20,20);


    for (int i=0;i<4;i++)
    {
        playersSprites[i].setTexture(textureBackgroundArt);
        playersSprites[i].setTextureRect(sf::IntRect(playersSpritesCords[i][0],
                                         playersSpritesCords[i][1], 280, 280));
        playersSprites[i].setPosition(playersSpritesCords[i][0], playersSpritesCords[i][1]);
    }


}

void Game::showMenu()
{
    musicMenu.play();
    musicMenu.setLoop(true);
    currentState = state_menu;
}
void Game::hideMenu()
{
    musicMenu.stop();
}

void Game::showGameBoard()
{
    //    musicGame.setVolume(20);
    musicGame.play();
    musicGame.setLoop(true);
    sfx.playLetsBegin();
    //    std::cout << "lets begin" << std::endl;
    currentState = state_lets_begin;
}

void Game::endGame()
{
    currentState = state_end_game;
    downTimeCounter = 0;
    numberFinishedPlayers = 4;
    setTxtEndGameAmount();
    //    musicBackground.stop();
}

/*!
 * \brief Game::handleLeftClick
 * \param pos
 * \param posFull
 * \param mousePos
 */
void Game::handleLeftClick(sf::Vector2f pos,sf::Vector2f posFull, int mousePos) {
    if (currentState==state_game)
    {
        std::array<int,2> movements = players[turn].getMovements(diceResultPlayer);
        if ((mousePos==movements[0]) || (mousePos==movements[1]))
        {
            players[turn].setFigurePos(mousePos);
            commandManager.processField(mousePos);
            const int *possibleExit = std::find(std::begin(efc::endPlayers),
                                                std::end(efc::endPlayers), mousePos);
            if (possibleExit != efc::endPlayers+4) {
                players[turn].done=true;
                players[turn].reachedPortal=true;
                commandManager.removeAllItems(turn);
                if (numberFinishedPlayers == 0)
                {
                    players[turn].reachedPortalFirst=true;
                    startDeerMode();
                }

                numberFinishedPlayers += 1;
                if (numberFinishedPlayers > 3)
                {
                    endGame();
                    return;
                }
            } else {
                //               std::cerr << "Not found" << std::endl;
            }
            nextPlayer();
            return;
        }
        //        std::string resultCommand = players[turn].getElem(posGui);
        //        command(resultCommand);
        //        commandManager.processGui(posGui);
    }
    else if (currentState==state_roll_dice)
    {
        sf::IntRect diceRect(roundDice.spriteDice.getGlobalBounds());
        if (diceRect.intersects(sf::IntRect(posFull.x, posFull.y, 1, 1)))
        {
            diceResultPlayer = roundDice.throwDiceSix();
            players[turn].characters[0].diceResult=diceResultPlayer;
            currentState = state_game;
            bubble.state = BubbleState::MOVE;
            nextRotateElem.reset();
            prevRotateElem.reset();
        }
    }

    if (currentState==state_menu)
    {
        downTimeCounter = 0;
        //        std::cout << " AA " <<downTimeCounter << std::endl;
        hideMenu();
        showGameBoard();
    }



    if (currentState==state_gui_end_round)
    {
        std::string resultCommand = guiRoundDice.getElem(pos);
        command(resultCommand);
    }

    if (currentState==state_lets_begin)
    {
        if (downTimeCounter>1)
        {
            currentState = state_roll_dice;
        }

    }

    if (currentState==state_end_game)
    {
        if (downTimeCounter>2)
        {
            currentState = state_menu;
            restartGame();
        }

    }


}

void Game::hideGameBoard()
{
    musicGame.play();
}
Game::Game():
    screenSize(efc::initScreenX,efc::initScreenY),
    viewFull(sf::FloatRect(00, 00, screenSize.x, screenSize.y)),
    viewGui(sf::FloatRect(00, 00, screenSize.x, screenSize.y)),
    viewTiles(sf::FloatRect(0, 0, 1360, 768)),
    selector(efc::TILE_SIZE),
    character(&textures, 3),
    gameTitle("deerportal"),
    roundDice(players),
    roundNumber(1),
    guiRoundDice(&textures),
    boardDiamonds(&textures),
    window(sf::VideoMode(efc::initScreenX, efc::initScreenY), "Deerportal - game about how human can be upgraded to the Deer"),
    turn(0),
    particleSystem( 430, 230),
    commandManager(*this),
    cardsDeck(&textures, &menuFont,&commandManager),
    deerModeCounter(4),
    deerModeActive(false)
{
    // TODO: perhaps get rid of the particles at all...
    particleSystem.setDissolve( true );
    particleSystem.setDissolutionRate( 10 );
    particleSystem.setShape( Shape::CIRCLE );
    particleSystem.fuel( 1000 );
    playersSpritesCords[0][0] = 202;
    playersSpritesCords[0][1] = 76;
    playersSpritesCords[1][0] = 562;
    playersSpritesCords[1][1] = 76;
    playersSpritesCords[3][0] = 202;
    playersSpritesCords[3][1] = 436;
    playersSpritesCords[2][0] = 562;
    playersSpritesCords[2][1] = 436;
    textLoading.setString("loading...");
    textLoading.setFont(menuFont);
    textLoading.setPosition(200,200);
    textLoading.setColor(sf::Color::White);
    textLoading.setCharacterSize(10);
    renderTexture.create(1360,768);
    renderTexture.clear(sf::Color::White);
    renderTexture.draw(textLoading);
    renderTexture.display();

    txtSurvivorsLabel.setFont(gameFont);
    txtLoosersLabel.setFont(gameFont);

    txtSurvivorsLabel.setCharacterSize(30);
    txtLoosersLabel.setCharacterSize(30);



    txtSurvivorsLabel.setString("Deer Mode Survivors");
    sf::FloatRect rectSurvivors = txtSurvivorsLabel.getLocalBounds();
    txtSurvivorsLabel.setPosition((1360/2)-(rectSurvivors.width/2),200);

    txtLoosersLabel.setString("Digested by The Elements");
    sf::FloatRect rectLoosers = txtLoosersLabel.getLocalBounds();
    txtLoosersLabel.setPosition((1360/2)-(rectLoosers.width/2),500);

    renderSprite.setTexture(renderTexture.getTexture());
    numberFinishedPlayers = 0;
    sf::Clock frameClock;
    guiRoundDice.active = true;
    showPlayerBoardElems = false;
    window.setVerticalSyncEnabled(true);

    std::srand (time(NULL));
    window.clear(sf::Color(55,55,55));
    renderTexture.draw(textLoading);
    window.display();

    loadAssets();
    textLoading.setFont(menuFont);
    textLoading.setPosition(200,200);
    textLoading.setColor(sf::Color::White);
    textLoading.setCharacterSize(10);
    renderTexture.clear(sf::Color::Black);
    renderTexture.draw(textLoading);
    window.display();

    initBoard();
    renderTexture.clear(sf::Color::Black);
    renderTexture.draw(textLoading);
    renderTexture.display();

    showMenu();
    //    currentState = state_end_game; //TODO: hacky debug hy


    // run the main loop
    while (window.isOpen())
    {
        sf::Time frameTime = frameClock.restart();

        // handle events
        sf::Event event;
        float xpos = 320.0f;
        float ypos = 240.0f;
        float xgrv = 0.0f;
        float ygrv = 0.0f;



        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                window.close(); break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Escape )
                    window.close();
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) )
                    particleSystem.fuel( 200/* * window.getFrameTime() */);
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
                    particleSystem.setPosition( --xpos, ypos );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
                    particleSystem.setPosition( ++xpos, ypos );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
                    particleSystem.setPosition( xpos, --ypos );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
                    particleSystem.setPosition( xpos, ++ypos );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) )
                    particleSystem.setGravity( --xgrv * 0.1f, ygrv * 0.1f);
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
                    particleSystem.setGravity( ++xgrv * 0.1f, ygrv * 0.1f );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) )
                    particleSystem.setGravity( xgrv * 0.1f, --ygrv * 0.1f );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) )
                    particleSystem.setGravity( xgrv * 0.1f, ++ygrv * 0.1f );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::G ) )
                    particleSystem.setGravity( 0.0f, 0.0f );
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::P ) )
                    particleSystem.setPosition( 320.0f, 240.0f );
                break;
            default:
                break;

            }

            sf::Vector2i localPositionTmp = sf::Mouse::getPosition(window);
            sf::Vector2f localPosition = window.mapPixelToCoords(localPositionTmp,viewTiles);
            //            sf::Vector2f localPositionGui = window.mapPixelToCoords(localPositionTmp,viewGui);
            sf::Vector2f localPositionFull = window.mapPixelToCoords(localPositionTmp,viewFull);
            int mousePosX = (int)localPosition.x / efc::TILE_SIZE;
            int mousePosY = (int)localPosition.y / efc::TILE_SIZE;
            int mousePos = efc::transCords(sf::Vector2i(mousePosX, mousePosY));
            if(event.type == sf::Event::Closed)
                window.close();

            // Showing mouse hover
            if (currentState==state_game)
            {
                if ((localPosition.x>efc::TILE_SIZE*efc::BOARD_SIZE) || (localPosition.x<0)  || (localPosition.y>efc::TILE_SIZE*efc::BOARD_SIZE) || (localPosition.y<0))
                {
                    showPlayerBoardElems = false;
                } else {
                    showPlayerBoardElems = true;
                }
            }

            if ((localPosition.x>=0) && (localPosition.y>=0) && (localPosition.x<=efc::BOARD_SIZE*efc::TILE_SIZE) && (localPosition.y<=efc::BOARD_SIZE*efc::TILE_SIZE))
            {
                selector.setPosition((int) (localPosition.x / efc::TILE_SIZE)*efc::TILE_SIZE, ((int) localPosition.y / efc::TILE_SIZE)*efc::TILE_SIZE);
            }

            /*!
             * Handling mouse click
             */
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    handleLeftClick(localPosition,
                                    localPositionFull, mousePos);
            }
        }
        update(frameTime);
        render(frameTime.asSeconds());
    }
}

void Game::update(sf::Time frameTime) {
    runningCounter += frameTime.asSeconds();
    if (currentState==state_game)
    {
        std::array<int,2> currentMovements = players[turn].getMovements(diceResultPlayer);
        if (currentMovements[0]>-1)
        {
            prevRotateElem.spriteRotate.setPosition(players[turn].characters[0].leftChar.getPosition());
            prevRotateElem.spriteRotate.move(10,20);

            // Modificator to fit on the bigger view
            prevRotateElem.spriteRotate.move(-202,-76);

            prevRotateElem.update(frameTime);
            prevRotateElem.setColor();
        }
        if (currentMovements[1]>-1)
        {
            nextRotateElem.spriteRotate.setPosition(players[turn].characters[0].rightChar.getPosition());
            nextRotateElem.spriteRotate.move(10,20);

            // Modificator to fit on the bigger view
            nextRotateElem.spriteRotate.move(-202,-76);
            nextRotateElem.update(frameTime);
            nextRotateElem.setColor();
        }
    }
    cardsDeck.update(frameTime);
    for (int i=0;i<4;i++)
    {

        players[i].play();
        players[i].update(frameTime);
    }

    if (currentState==state_lets_begin)
    {
        downTimeCounter += frameTime.asSeconds();
        spriteLestBegin.setColor(sf::Color(255,255,255,255-(downTimeCounter*35)));
        if (downTimeCounter>5)
        {
            currentState = state_roll_dice;
            bubble.state = BubbleState::DICE;
        }
    }

    if (currentState==state_end_game)
    {
        downTimeCounter += frameTime.asSeconds();

    }

    bubble.update(frameTime);
}

/*!
 * \brief Game::nextRound is happening each every 4 months
 */
void Game::nextRound() {
    turn = 0;
    //    std::string result = roundDice.drawRound();
    roundNumber += 1;
    month++;
    if (month==13)
        month=1;
    if (month%4==0)
        currentSeason++;
    if (currentSeason>3)
        currentSeason=0;
//    if (players[turn].done==true)
//        nextPlayer();

    launchNextPlayer();
}

/*!
 * \brief Game::nextPlayer calculates which player should play
 */
void Game::nextPlayer(){

    // End of game - we don't calculate more players
    if (currentState==state_end_game)
    {
        return;
    }

    // End of game - we don't calculate more players
    if (numberFinishedPlayers==4)
    {
        //        std::cout << "Everybody Finished!!!" << std::endl;
        endGame();
        return ;
    }

    // Update old player

    players[turn].updatePlayer();
    if (turn>2)
    {
        nextRound();
        return;
    }

    turn++;


//    if (players[turn].done==true)
//    {
//        //        std::cout << "Player " << turn << " is done" << std::endl;
//        nextPlayer();
//        return;
//    }





    launchNextPlayer();

}
void Game::launchNextPlayer(){
    if (deerModeActive)
    {
        deerModeCounter -= 1;

    }

    if (deerModeCounter<0)
    {
        endGame();
        return ;
    }
    // Just control
    if (players[turn].done==true)
    {
        //        std::cout << "Player " << turn << " is done" << std::endl;
        nextPlayer();
        return;
    }

    // Frozen player
    if (players[turn].frozenLeft>0)
    {
        players[turn].frozenLeft -= 1;
        nextPlayer();
        return;
    }


    // Here we process new player

    selector.changeColor(turn);
    for (int i=0;i<4;i++)
    {
        if (i==turn)
        {
            players[i].setActive(true);
            currentNeighbours = players[i].getNeighbours();
        }
        else
            players[i].setActive(false);
    }

    sfxClick.play();
    diceResultPlayer = 6;
    roundDice.setDiceTexture(diceResultPlayer);

    players[turn].characters[0].diceResult = diceResultPlayer;
    groupHud.setRoundName(roundNumber);
    if (deerModeActive==false)
    {
        groupHud.setSeason(currentSeason);
        groupHud.setMonthName(month%4);
    } else
    {
        groupHud.setDeerModeActive();
        int number = (deerModeCounter/4);

        if (deerModeCounter<16)
            number += 1;
        if (number<0)
        {
            endGame();
            return;
        }
        groupHud.setDeerModeCounter(number);
    }
    currentState = state_roll_dice;
    bubble.state = BubbleState::DICE;
    roundDice.setColor(turn);
    bubble.setPosition(players[turn].characters[0].getPosition().x-30,
            players[turn].characters[0].getPosition().y-45);
}

void Game::drawPlayersGui(){
    for (int i=0;i<4;i++)
    {
        renderTexture.draw(players[i]);
    }
}

void Game::drawSquares() {
    if (showPlayerBoardElems)
    {
        renderTexture.draw(selector);
    }

}

void Game::drawBaseGame()
{
    renderTexture.setView(viewTiles);
    for (int i=0;i<4;i++)
    {
        renderTexture.draw(players[i].elems);
    }
    drawSquares();
    renderTexture.setView(viewGui);
    renderTexture.setView(viewTiles);
    renderTexture.setView(viewTiles); // Yeah Katia's inspiration
    shaderBlur.setParameter("blur_radius", sin(runningCounter*0.01f) );

    //    renderTexture.draw(gameBackground);
    renderTexture.setView(viewFull);
    //    renderTexture.draw(spriteBackgroundArt,  &shaderDark);
    //    spriteBackgroundArt.setColor(sf::Color(255, 255, 255, 208));
    shaderBlur.setParameter("blur_radius", 0.01);
    //    shaderBlur.setParameter("blur_radius", sin(runningCounter*0.01) );
    //    shaderBlur.setParameter("blur_radius", sin(runningCounter*0.01) );
    shaderPixel.setParameter("pixel_threshold", sin(runningCounter* 0.005f));

    renderTexture.draw(spriteBackgroundArt);
    spriteBackgroundArt.setColor(sf::Color(255, 255, 255));
    shaderBlur.setParameter("blur_radius", sin(runningCounter* 0.05f)/2);

    renderTexture.draw(cardsDeck);
    if (currentState==state_roll_dice)
    {
        spriteBackgroundArt.setColor(sf::Color(255, 255, 255));
        shaderBlur.setParameter("blur_radius", sin(runningCounter* 0.5f)/4);
        renderTexture.draw(roundDice.spriteDice, &shaderBlur);
    }
    else
        renderTexture.draw(roundDice.spriteDice);
    renderTexture.setView(viewTiles);
    drawSquares();

}

void Game::drawCharacters(){
    if (currentState==state_game)
    {
        std::array<int,2> currentMovements = players[turn].characters[0].getMovements(diceResultPlayer);
        if (currentMovements[1]>-1)
        {
            if (nextRotateElem.active)
                renderTexture.draw(nextRotateElem);
        }
        if (currentMovements[0]>-1)
        {
            if (prevRotateElem.active)
                renderTexture.draw(prevRotateElem);
        }
    }
    renderTexture.setView(viewFull);
    shaderBlur.setParameter("blur_radius", 0.005f);
    for (int i=0;i<4;i++)
    {
        for (auto&& j: players[i].characters)
        {
            if (currentState==state_game)
                j.drawMovements = true;
            else
                j.drawMovements = false;
            renderTexture.draw(j);
            //            renderTexture.draw(j, &shaderBlur);
        }
    }
}

/*!
 * \brief Game::render main function responsible for drawing all elements
 * \param deltaTime
 */
void Game::render(float deltaTime)
{
    window.clear();
    renderTexture.clear();
    if ((currentState==state_game) or (currentState==state_roll_dice))
    {
        renderTexture.setView(viewFull);
        renderTexture.draw(spriteBackgroundDark);
        renderTexture.setView(viewTiles);
        drawBaseGame();
        renderTexture.setView(viewFull);
        renderTexture.draw(groupHud);
        renderTexture.setView(viewTiles);
        drawCharacters();
        renderTexture.draw(boardDiamonds);
        renderTexture.draw(bubble);
        renderTexture.setView(viewFull);
        drawPlayersGui();
        renderTexture.setView(viewFull);
    } else if (currentState==state_gui_elem) {
        renderTexture.setView(viewFull);
        shaderBlur.setParameter("blur_radius", 2);
        renderTexture.draw(spriteBackgroundDark, &shaderBlur);
        drawBaseGame();
        drawCharacters();
        renderTexture.setView(viewFull);
        renderTexture.draw(groupHud);

    }  else if (currentState==state_menu) {
        shaderBlur.setParameter("blur_radius", 15);
        renderTexture.draw(menuTxt, &shaderBlur);
        renderTexture.draw(menuTxt);
        renderTexture.draw(paganHolidayTxt);
    }  else if (currentState==state_lets_begin) {
        renderTexture.setView(viewFull);
        shaderBlur.setParameter("blur_radius", 4);
        renderTexture.draw(spriteBackgroundDark, &shaderBlur);
        renderTexture.setView(viewTiles);
        drawBaseGame();
        drawCharacters();
        renderTexture.draw(boardDiamonds, &shaderBlur);
        renderTexture.setView(viewFull);
        drawPlayersGui();
        renderTexture.draw(spriteLestBegin,&shaderBlur);

    } else if (currentState==state_gui_end_round){
        renderTexture.setView(viewFull);
        renderTexture.draw(spriteBackgroundDark);
        drawBaseGame();
        shaderBlur.setParameter("blur_radius", 0.05f);
        renderTexture.draw(guiRoundDice, &shaderBlur);
        renderTexture.setView(viewFull);
        renderTexture.draw(groupHud);
    }
    else if (currentState==state_end_game){
        renderTexture.setView(viewFull);
        renderTexture.draw(spriteBackgroundDark);
        renderTexture.draw(spriteLestBegin,&shaderBlur);
        renderTexture.draw(endGameTxt);

//        for (int i=0;i<4;i++){
//            if (players[i].reachedPortal)
//                renderTexture.draw(endGameTxtAmount[i]);
//        }

        renderTexture.draw(txtWinner);
        renderTexture.draw(txtSurvivorsLabel);
        for (int i=0; i<txtSurvivors.size();i++) {
            renderTexture.draw(txtSurvivors[i]);
        }
        renderTexture.draw(txtLoosersLabel);
        for (int i=0; i<txtLoosers.size();i++) {
            renderTexture.draw(txtLoosers[i]);
        }

    }

    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());
    shaderBlur.setParameter("blur_radius", sin(deltaTime)*0.015f);
    shaderBlur.setParameter("blur_radius", 0.0003f);
    window.draw(renderSprite, &shaderBlur);


    particleSystem.remove();
    particleSystem.update();
    particleSystem.render();
    window.draw( particleSystem.getSprite() );


    window.display();
}

void Game::command(std::string command){
    if (command.compare("end_of_round")==0)
    {
        std::string subResult = command.substr(13);
        guiRoundDice.active = true;
        guiRoundDice.setTitle(subResult);
        currentState = state_gui_end_round;
    }

    if (command=="end_turn")
        nextPlayer();

}

/*!
 * \brief Game::startDeerMode launches last episode of the game
 */
void Game::startDeerMode() {
    deerModeActive = true;
    deerModeCounter = 16;
}
}



