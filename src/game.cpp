#include "game.h"
#include "particle.h"
#include "calendar.h"
#include <algorithm>


namespace DP {

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
            return true;

        } else
        {
            return false;
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
    //    int startHeight = 100;
    int separator = 40;
    std::array<ResultTable, 4> results = {
        {
            ResultTable(0,players[0].cash, players[0].reachedPortal,players[0].reachedPortalFirst),
            ResultTable(1,players[1].cash, players[1].reachedPortal,players[1].reachedPortalFirst),
            ResultTable(2,players[2].cash, players[2].reachedPortal,players[2].reachedPortalFirst),
            ResultTable(3,players[3].cash, players[3].reachedPortal,players[3].reachedPortalFirst)
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
        sf::Text tmpText;
        tmpText.setFont(gameFont);
        tmpText.setCharacterSize(25);
        tmpText.setString(elementName+ " " + std::to_string(players[playerNumber].cash));
        sf::FloatRect rectTxt = tmpText.getLocalBounds();

        if (players[playerNumber].reachedPortal==true)
        {
            int counter = txtSurvivors.size();

            tmpText.setPosition((1360/2)-(rectTxt.width/2),200+(counter*separator));
            txtSurvivors.push_back(tmpText);
        } else
        {
            int counter = txtLoosers.size();

            tmpText.setPosition((width/2)-(rectTxt.width/2),540+(counter*separator));
            txtLoosers.push_back(tmpText);
        }

    }
    if (txtSurvivors.size()>0)
    {
        txtWinner.setString("Winner: " + txtSurvivors[0].getString());
        txtSurvivors.erase(txtSurvivors.begin()+0);
    }
    txtWinner.setCharacterSize(40);
    sf::FloatRect rectTxt = txtWinner.getLocalBounds();
    txtWinner.setPosition((1360/2)-(rectTxt.width/2),120);

}

void Game::initBoard()
{

    time_t t = time(0);
    struct tm * now = localtime( & t );
    int month = now->tm_mon + 1;
    int day = now->tm_mday;
    paganHolidayString =  getHoliday(month, day);
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
    spriteBigDiamond.setTexture(textures.textureBigDiamond);
    spriteBigDiamond.setPosition(474,342);
    spriteBigDiamond.setColor(sf::Color (255, 255, 255,196));
    restartGame();
    launchNextPlayer();

    endGameTxt.setFont(gameFont);
    endGameTxt.setString("End of the Game");
    endGameTxt.setCharacterSize(30);

    sf::FloatRect ss = endGameTxt.getLocalBounds();
    endGameTxt.setPosition((1360/2)-(ss.width/2),60);

    setTxtEndGameAmount();
    bubble.setPosition(players[turn].characters[0].getPosition().x-30,
            players[turn].characters[0].getPosition().y-45);

    txtSurvivorsLabel.setString("Survivors");
    txtSurvivorsLabel.setFont(gameFont);
    txtSurvivorsLabel.setCharacterSize(30);
    sf::FloatRect rectSurvivors = txtSurvivorsLabel.getLocalBounds();
    txtSurvivorsLabel.setPosition((1360/2)-(rectSurvivors.width/2),200);

    txtLoosersLabel.setString("Digested by The Elements");
    txtLoosersLabel.setFont(gameFont);
    txtLoosersLabel.setCharacterSize(30);
    sf::FloatRect rectLoosers = txtLoosersLabel.getLocalBounds();
    txtLoosersLabel.setPosition((1360/2)-(rectLoosers.width/2),500);
    credits.setTxt(0);

}

/*!
 * \brief Game::restartGame sets all the variables to the default value
 */
void Game::restartGame()
{

    Player playerHud1(&textures, &gameFont,0);
    Player playerHud2(&textures, &gameFont,1);
    Player playerHud3(&textures, &gameFont,2);
    Player playerHud4(&textures, &gameFont,3);
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
    roundNumber = 1;
    month = 0;
    cardsDeck.reloadCards();
    deerModeActive = false;
    deerModeCounter = 16;

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

    if (!gameFont.loadFromFile(ASSETS_PATH"assets/fnt/metal-mania.regular.ttf"))
    {
        std::exit(1);
    }
    if (!menuFont.loadFromFile(ASSETS_PATH"assets/fnt/metal-macabre.regular.ttf"))
    {
        std::exit(1);
    }

    menuBackground.setTexture(textures.textureMenu);


    spriteDeerGod.setTexture(textures.textureDeerGod);

    if (!shaderBlur.loadFromFile(ASSETS_PATH"assets/shaders/blur.frag", sf::Shader::Fragment))
        std::exit(1);

    if (!shaderPixel.loadFromFile(ASSETS_PATH"assets/shaders/pixelate.frag", sf::Shader::Fragment))
        std::exit(1);
    if (!shaderDark.loadFromFile(ASSETS_PATH"assets/shaders/dark.frag", sf::Shader::Fragment))
        std::exit(1);

    if (!textureBackgroundArt.loadFromFile(ASSETS_PATH"assets/img/background_land.png"))
        std::exit(1);

    if (!musicGame.openFromFile(ASSETS_PATH"assets/audio/game.ogg"))
        std::exit(1);
    //    if (!musicBackground.openFromFile(ASSETS_PATH"assets/audio/wind2.ogg"))
    //        std::exit(1);
    if (!musicMenu.openFromFile(ASSETS_PATH"assets/audio/menu.ogg"))
        std::exit(1);


    if (!sfxClickBuffer.loadFromFile(ASSETS_PATH"assets/audio/click.ogg"))
        std::exit(1);

    if (!sfxDoneBuffer.loadFromFile(ASSETS_PATH"assets/audio/done.ogg"))
        std::exit(1);
    //    if (!textureBackground.loadFromFile(ASSETS_PATH"assets/img/background.png"))
    //        std::exit(1);

    spriteBackgroundArt.setTexture(textureBackgroundArt);
    menuTxt.setFont(gameFont);
    menuTxt.setCharacterSize(60);
    menuTxt.setString(gameTitle);
    int width = menuTxt.getLocalBounds().width;
    int height = menuTxt.getLocalBounds().height;
    menuTxt.setPosition(1050-(width/2),750-(height/2)-150);
    menuTxt.setFillColor(sf::Color(255, 255, 255, 85));
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

    currentState = state_setup_players;
    //    currentState = state_lets_begin;
}

void Game::endGame()
{
    musicGame.stop();
    currentState = state_end_game;
    downTimeCounter = 0;
    numberFinishedPlayers = 4;
    setTxtEndGameAmount();
    //    musicBackground.stop();
}

void Game::throwDiceMove() {
    // Throw a dice action
    diceResultPlayer = roundDice.throwDiceSix();
    players[turn].characters[0].diceResult=diceResultPlayer;
    currentState = state_game;
    bubble.state = BubbleState::MOVE;
    nextRotateElem.reset();
    prevRotateElem.reset();
}
/*!
 * \brief Game::playerMakeMove move the player into the position on the map
 * \param mousePos
 */
void Game::playerMakeMove(int mousePos) {
    players[turn].setFigurePos(mousePos);
    commandManager.processField(mousePos);
    const int *possibleExit = std::find(std::begin(DP::endPlayers),
                                        std::end(DP::endPlayers), mousePos);
    if (possibleExit != DP::endPlayers+4) {
        players[turn].done=true;
        players[turn].reachedPortal=true;
        commandManager.removeAllItems(turn);
        if (numberFinishedPlayers == 0)
        {
            players[turn].reachedPortalFirst=true;
            int turnover =  (rand() %  2)+5;

            players[turn].cash += turnover;
            players[turn].updatePlayer();
            startDeerMode();
        }

        sfx.soundPortal.play();
        numberFinishedPlayers += 1;
        if (numberFinishedPlayers > 3)
        {
            endGame();
            return;
        }
    }
    nextPlayer();
    return;
}

int Game::mostDiamonds() const
{
    std::array<int,4> results = {{players[0].cash,players[1].cash,players[2].cash,players[3].cash}};
    auto minmax = std::minmax_element(std::begin(results), std::end(results));
    int maxResult = *(minmax.second);
    int result = 0;
    int pos = -1;
    for (int i=0; i<4;i++)
    {
        if (players[i].cash == maxResult)
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

/*!
 * \brief Game::handleLeftClick
 * \param pos
 * \param posFull
 * \param mousePos
 */
void Game::handleLeftClick(sf::Vector2f pos,sf::Vector2f posFull, int mousePos) {
    if (currentState==state_game)
    {
        if (players[turn].human)
        {
            std::array<int,2> movements = players[turn].getMovements(diceResultPlayer);
            if ((mousePos==movements[0]) || (mousePos==movements[1]))
            {
                playerMakeMove(mousePos);

            }
        }
    }

    if (currentState==state_setup_players)
    {
        for (int i=0;i<4;i++)
        {
            sf::IntRect spriteHumanRect(players[i].spriteAI.getGlobalBounds());
            if (spriteHumanRect.intersects(sf::IntRect(posFull.x, posFull.y, 1, 1)))
            {
                players[i].swapHuman();
            }
        }
        sf::IntRect startGameRect(580,640,180,80);
        if (startGameRect.intersects(sf::IntRect(posFull.x, posFull.y, 1, 1)))
        {
            bigDiamondActive = true;
            banner.setText("start game");
            currentState=state_roll_dice;
            launchNextPlayer();
            return;
        }
    }
    else if (currentState==state_roll_dice)
    {
        if (players[turn].human){
            sf::IntRect diceRect(roundDice.spriteDice.getGlobalBounds());
            if (diceRect.intersects(sf::IntRect(posFull.x, posFull.y, 1, 1)))
            {
                throwDiceMove();
            }
        }
    }

    if (currentState==state_menu)
    {
        downTimeCounter = 0;
        hideMenu();
        showGameBoard();
        return;
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
            restartGame();
            launchNextPlayer();
            return;
        }
    }

    if (currentState==state_end_game)
    {
        if (downTimeCounter>2)
        {
            currentState = state_menu;
            musicMenu.play();
            restartGame();
            return ;
            //            restartGame();
        }
    }


}

Game::Game(bool newTestMode):
    screenSize(DP::initScreenX,DP::initScreenY),
    viewFull(sf::FloatRect(00, 00, screenSize.x, screenSize.y)),
    viewGui(sf::FloatRect(00, 00, screenSize.x, screenSize.y)),
    viewTiles(sf::FloatRect(0, 0, 1360, 768)),
    selector(DP::TILE_SIZE),
    character(&textures, 3),
    gameTitle("deerportal"),
    roundDice(players),
    roundNumber(1),
    guiRoundDice(&textures),
    boardDiamonds(&textures),
    window(sf::VideoMode(DP::initScreenX, DP::initScreenY), "Deerportal - game about how human can be upgraded to the Deer"),
    turn(0),
    oscilator(-1),
    oscilatorInc(true),
    particleSystem( 430, 230),
    commandManager(*this),
    cardsDeck(&textures, &menuFont,&commandManager),
    banner(&gameFont),
    bigDiamondActive(false),
    credits(&gameFont),
    cpuTimeThinkingInterval(1.0f),
    deerModeCounter(4),
    deerModeActive(false),
    gameVersion()
{
    testMode = newTestMode;
    // TODO: perhaps get rid of the particles at all...
    particleSystem.setDissolve( true );
    particleSystem.setDissolutionRate( 10 );
    particleSystem.setShape( DP::CIRCLE );
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
    textLoading.setFillColor(sf::Color::White);
    textLoading.setCharacterSize(10);
    renderTexture.create(1360,768);
    renderTexture.clear(sf::Color::White);
    renderTexture.draw(textLoading);
    renderTexture.display();

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
    textLoading.setFillColor(sf::Color::White);
    textLoading.setCharacterSize(10);
    renderTexture.clear(sf::Color::Black);
    renderTexture.draw(textLoading);
    window.display();

    gameVersion.setString("version: " + std::string(DEERPORTAL_VERSION)+"-"+std::string(BASE_PATH));
    gameVersion.setFont(gameFont);
    gameVersion.setPosition(10,10);
    gameVersion.setFillColor(sf::Color::White);
    gameVersion.setCharacterSize(15);
 

    initBoard();
    renderTexture.clear(sf::Color::Black);
    renderTexture.draw(textLoading);
    renderTexture.display();

    showMenu();

    // run the main loop

    if (testMode){

        std::exit(0);
    }

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
            int mousePosX = (int)localPosition.x / DP::TILE_SIZE;
            int mousePosY = (int)localPosition.y / DP::TILE_SIZE;
            int mousePos = DP::transCords(sf::Vector2i(mousePosX, mousePosY));
            if(event.type == sf::Event::Closed)
                window.close();

            // Showing mouse hover
            if (currentState==state_game)
            {
                if ((localPosition.x>DP::TILE_SIZE*DP::BOARD_SIZE) || (localPosition.x<0)  || (localPosition.y>DP::TILE_SIZE*DP::BOARD_SIZE) || (localPosition.y<0))
                {
                    showPlayerBoardElems = false;
                } else {
                    showPlayerBoardElems = true;
                }
            }

            if ((localPosition.x>=0) && (localPosition.y>=0) && (localPosition.x<=DP::BOARD_SIZE*DP::TILE_SIZE) && (localPosition.y<=DP::BOARD_SIZE*DP::TILE_SIZE))
            {
                selector.setPosition((int) (localPosition.x / DP::TILE_SIZE)*DP::TILE_SIZE, ((int) localPosition.y / DP::TILE_SIZE)*DP::TILE_SIZE);
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
    banner.update(frameTime);
    credits.update(frameTime);
    runningCounter += frameTime.asSeconds();

    cpuTimeThinking -= frameTime.asSeconds();


    if (oscilatorInc)
    {
        oscilator += frameTime.asSeconds();
    } else {
        oscilator -= frameTime.asSeconds();
    }

    if (oscilator<-1)
        oscilatorInc = true;

    if (oscilator>1)
        oscilatorInc = false;


    float modifier = sin(oscilator/2.5)*30.0f;
    spriteBigDiamond.setPosition(474,342+modifier);



    if (currentState==state_roll_dice)
    {
        if ((cpuTimeThinking<0) && (players[turn].human==false))
        {
            cpuTimeThinking = cpuTimeThinkingInterval;
            throwDiceMove();
        }
    }

    if (currentState==state_game)
    {
        std::array<int,2> currentMovements = players[turn].getMovements(diceResultPlayer);

        if ((cpuTimeThinking<0) &&  (players[turn].human==false))
        {
            std::vector<int> listRandomPos;



            if (currentMovements[0]>-1) {
                listRandomPos.push_back(currentMovements[0]);

            }
            if (currentMovements[1]>-1) {
                listRandomPos.push_back(currentMovements[1]);

            }

            unsigned int sizeRndPos = listRandomPos.size();
            if (sizeRndPos==0) {

            } else if  (sizeRndPos==1) {
                playerMakeMove(listRandomPos[0]);

            } else if (sizeRndPos==2) {

                if (deerModeActive)
                {
                    playerMakeMove(listRandomPos[1]);
                } else
                {


                    if (players[turn].reachPortalMode == true)
                    {
                        playerMakeMove(listRandomPos[1]);
                    }
                    else
                    {
                        if (boardDiamonds.ifFieldIsEmpty(listRandomPos[1])==false)
                        {
                            playerMakeMove(listRandomPos[1]);
                            return;

                        }
                        if (boardDiamonds.ifFieldIsEmpty(listRandomPos[0])==false)
                        {
                            playerMakeMove(listRandomPos[0]);
                            return;

                        }
                        if ((boardDiamonds.ifFieldIsEmpty(listRandomPos[0])==false) && (boardDiamonds.ifFieldIsEmpty(listRandomPos[1])==false))

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

    cpuTimeThinking = cpuTimeThinkingInterval;
    if (mostDiamonds()==turn)
    {
        players[turn].reachPortalMode = true;
    } else {
        players[turn].reachPortalMode = false;

    }

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
        renderTexture.draw(roundDice.spriteDice);
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
    if ((currentState==state_game) || (currentState==state_roll_dice))
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
    } else if (currentState==state_setup_players) {
        renderTexture.setView(viewFull);
        renderTexture.draw(spriteDeerGod);
        for (int i=0;i<4;i++)
        {
            renderTexture.draw(players[i].spriteAI);
        }
    } else if (currentState==state_gui_elem) {
        renderTexture.setView(viewFull);
        shaderBlur.setParameter("blur_radius", 2);
        renderTexture.draw(spriteBackgroundDark, &shaderBlur);
        drawBaseGame();
        drawCharacters();
        renderTexture.setView(viewFull);
        renderTexture.draw(groupHud);

    }  else if (currentState==state_menu) {


//        shaderBlur.setParameter("blur_radius", 15);
        renderTexture.draw(menuBackground);
//        //        renderTexture.draw(menuTxt, &shaderBlur);
        //        renderTexture.draw(menuTxt);
        renderTexture.draw(paganHolidayTxt);
        renderTexture.draw(gameVersion);
        renderTexture.draw(credits);
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
        for (unsigned int i=0; i<txtSurvivors.size();i++) {
            renderTexture.draw(txtSurvivors[i]);
        }
        renderTexture.draw(txtLoosersLabel);
        for (unsigned int i=0; i<txtLoosers.size();i++) {
            renderTexture.draw(txtLoosers[i]);
        }

    }
    if (bigDiamondActive)
        renderTexture.draw(spriteBigDiamond);
    if (banner.active)
        renderTexture.draw(banner);



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
    banner.setText("deer mode");
    bigDiamondActive = false;
    sfx.soundDeerMode.play();
}
}




