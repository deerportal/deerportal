#include "game.h"
#include "particle.h"
#include "calendar.h"


namespace efc {

int initScreenX = 1360;
int initScreenY = 768;
int currentSeason = 1;
int month = 0;


struct ResultTable
{
    int playerNumber;
    int playerResult;
    ResultTable(int number,int result) : playerNumber(number), playerResult(result) {}
    bool operator < (const ResultTable& otherResult) const
    {
        return (playerResult > otherResult.playerResult);
    }
};


void Game::setTxtEndGameAmount(){


    std::string elementNames[4] = {"Water","Earth", "Fire", "Air"};

    int width=1360;
//    int height = 768;
    int separator = 40;
    std::array<ResultTable, 4> results = {
        {
            ResultTable(0,players[0].cash),
            ResultTable(1,players[1].cash),
            ResultTable(2,players[2].cash),
            ResultTable(3,players[3].cash)
        }
    };

//    std::array <int, 4> results = {{players[0].cash,players[1].cash,players[2].cash,players[3].cash}};



std::vector < ResultTable > resultsVector;
for (int i=0;i<4;i++)
{
    resultsVector.push_back(results[i]);
};

std::sort(resultsVector.begin(), resultsVector.end());


    for (int i=0;i<4;i++)
    {

        int playerNumber = resultsVector[i].playerNumber;
        std::string elementName = elementNames[playerNumber];

        std::string label = elementName+ " " + std::to_string(players[playerNumber].cash);
        endGameTxtAmount[i].setString(label);
        sf::FloatRect ss = endGameTxtAmount[i].getLocalBounds();
        endGameTxtAmount[i].setPosition((width/2)-(ss.width/2),separator+(i*separator));



//        std::string label = elementNames[i]+ " " + std::to_string(players[i].cash);
//        endGameTxtAmount[i].setString(label);
//        sf::FloatRect ss = endGameTxtAmount[i].getLocalBounds();
//        endGameTxtAmount[i].setPosition((width/2)-(ss.width/2),separator+(i*separator));

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
//    spriteBackground.setTexture(textureBackground);
    spriteBackgroundDark.setTexture(textures.backgroundDark);
    spriteBackgroundDark.setPosition(0,0);
//    gameBackground.setTexture(textures.textureGameBackground);
    spriteLestBegin.setTexture(textures.textureLetsBegin);
    viewTiles.setViewport(sf::FloatRect(0.15f,0.1f, 1.0f, 1.0f));
    viewGui.setViewport(sf::FloatRect(0.806f,0.066f, 1, 1));

    groupHud.setFont(&gameFont);
    groupHud.setSeason(currentSeason);
    groupHud.setRoundName(roundNumber);

//    sf::IntRect seasonsRect[4] = {sf::IntRect(0,0,255,255), sf::IntRect(256,0,512,255), sf::IntRect(0,255, 255, 512), sf::IntRect(255,255,512, 512)};

//    sf::Sprite season1;
//    season1.setTexture(textures.textureSeasons);
//    season1.setTextureRect(seasonsRect[0]);
//    season1.setPosition(0,400);
//    season1.scale(sf::Vector2f(0.25f, 0.25f));
//    season1.move(37.5, 30.5);
//    season1.setColor(sf::Color(0,255,0,80));
//    seasons[0] = season1;

//    sf::Sprite season2;
//    season2.setTexture(textures.textureSeasons);
//    season2.setTextureRect(seasonsRect[1]);
//    season2.setPosition(0,400);
//    season2.scale(sf::Vector2f(0.25f, 0.25f));
//    season2.move(37.5, 30.5);
//    season2.setColor(sf::Color(200,200,50,80));
//    seasons[1] = season2;

//    sf::Sprite season3;
//    season3.setTexture(textures.textureSeasons);
//    season3.setTextureRect(seasonsRect[2]);
//    season3.setPosition(0,400);
//    season3.scale(sf::Vector2f(0.25f, 0.25f));
//    season3.move(37.5, 30.5);
//    season3.setColor(sf::Color(90,90,255,80));
//    seasons[2] = season3;

//    sf::Sprite season4;
//    season4.setTexture(textures.textureSeasons);
//    season4.setTextureRect(seasonsRect[3]);
//    season4.setPosition(0,400);
//    season4.scale(sf::Vector2f(0.25f, 0.25f));
//    season4.move(37.5, 30.5);
//    season4.setColor(sf::Color(255,0,0,80));
//    seasons[3] = season4;

    // Initialization of the players
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
       boardDiamonds.reorder(i);

    }
    turn = 0;
    currentSeason = 1;
    month = 0;
    cardsDeck.reloadCards();
}

void Game::setCurrentNeighbours ()
{
    currentNeighbours = players[turn].getNeighbours();
}

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
//    musicBackgroun/*d*/.play();
//    musicBackground.setLoop(true);
//    menuBackground.setTexture(textures.textureMenu);
//    musicBackground.setVolume(7);
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
    setTxtEndGameAmount();
//    musicBackground.stop();
}

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
                numberFinishedPlayers += 1;
                if (numberFinishedPlayers > 3)
                    endGame();
            } else {
                //               std::cerr << "Not found" << std::endl;
            }
            nextPlayer();
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
    cardsDeck(&textures, &menuFont,&commandManager)
{


    //particleSystem.setPosition( width/2, height/2 );
    //particleSystem.setGravity( 1.0f, 1.0f );
    //particleSystem.setParticleSpeed( 80.0f );
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
//        std::string resultCommand = "";
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
//            if (currentState==state_gui_elem)
//            {
//                resultCommand = guiSelectBuilding.getElem(localPositionFull);
//                showPlayerBoardElems = false;
//                if (resultCommand.find("elem_")==0)
//                    command(resultCommand);
//                else
//                    command("hide_gui_elem_description");
//            }

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
            prevRotateElem.spriteRotate.move(4,16);
            prevRotateElem.spriteRotate.move(-202,-76);

            prevRotateElem.update(frameTime);
            prevRotateElem.setColor(turn);
        }
        if (currentMovements[1]>-1)
        {
            nextRotateElem.spriteRotate.setPosition(players[turn].characters[0].rightChar.getPosition());
            nextRotateElem.spriteRotate.move(4,16);
            nextRotateElem.spriteRotate.move(-202,-76);
            nextRotateElem.update(frameTime);
            nextRotateElem.setColor(turn);
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
    if (players[turn].done==true)
        nextPlayer();
}

void Game::nextPlayer(){

    if (currentState==state_end_game)
    {
        return;
    }



    if (numberFinishedPlayers==4)
    {
//        std::cout << "Everybody Finished!!!" << std::endl;
        endGame();

    }

    if (turn<4)
        players[turn].updatePlayer();
    else
        nextRound();
    turn++;
    if ((players[turn].done==true) && (turn<4))
    {
//        std::cout << "Player " << turn << " is done" << std::endl;
        nextPlayer();
    }

    if ((turn==4) || (turn>4))
    {
        nextRound();
    }

    if (players[turn].frozenLeft>0)
    {
        players[turn].frozenLeft -= 1;
        nextPlayer();
    }

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
    groupHud.setSeason(currentSeason);
    groupHud.setMonthName(month%4);
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
    //    window.draw(map);
    for (int i=0;i<4;i++)
    {
        renderTexture.draw(players[i].elems);
    }
    drawSquares();
    renderTexture.setView(viewGui);
    renderTexture.setView(viewTiles);
    renderTexture.setView(viewTiles); // Yeah Katia's inspiration
    shaderBlur.setParameter("blur_radius", sin(runningCounter*0.01f) );

    renderTexture.draw(gameBackground);
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

//    for (int i=0;i<4;i++)
//    {
//        sf::RectangleShape rectangle(sf::Vector2f(284, 284));
//        rectangle.setPosition(playersSpritesCords[i][0]-2,playersSpritesCords[i][1]-2);
//        rectangle.setFillColor(sf::Color(0, 0, 0,55));
//        renderTexture.draw(rectangle);
//         if (turn==i)
//         renderTexture.draw(playersSprites[i]);
//         else
//             renderTexture.draw(playersSprites[i], &shaderBlur);
//    }
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
            renderTexture.draw(nextRotateElem);
        if (currentMovements[0]>-1)
            renderTexture.draw(prevRotateElem);
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



void Game::render(float deltaTime)
{
    window.clear();
    renderTexture.clear();
    if (currentState==state_game)
    {
        renderTexture.setView(viewFull);
        shaderBlur.setParameter("blur_radius", 2);

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
//        renderTexture.draw(groupHud);

    } else if (currentState==state_roll_dice) {
        renderTexture.setView(viewFull);
        shaderBlur.setParameter("blur_radius", 2);
        renderTexture.draw(spriteBackgroundDark, &shaderBlur);

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
//        renderTexture.draw(groupHud);

    } else if (currentState==state_gui_elem) {
        renderTexture.setView(viewFull);
        shaderBlur.setParameter("blur_radius", 2);
        renderTexture.draw(spriteBackgroundDark, &shaderBlur);

        drawBaseGame();
        drawCharacters();
//        window.draw(guiSelectBuilding);
        renderTexture.setView(viewFull);
        renderTexture.draw(groupHud);

    }  else if (currentState==state_menu) {
        shaderBlur.setParameter("blur_radius", 15);
        renderTexture.draw(menuTxt, &shaderBlur);
        renderTexture.draw(menuTxt);
        renderTexture.draw(paganHolidayTxt);
//        window.draw(menuTxt);
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

    for (int i=0;i<4;i++){
        renderTexture.draw(endGameTxtAmount[i]);
    }

//    drawBaseGame();

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
//    if (command=="close_gui")
//    {
//        guiSelectBuilding.active = false;
//        currentState=state_game;
//        sfxClick.play();
//    }
//    if (command=="hide_gui_elem_description")
//    {
//        if (currentState==state_gui_elem) {
//            guiSelectBuilding.descriptionActive = false;
//        }
//    }

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

sf::Vector2f Game::getMousePos(){
    sf::Vector2i mousePosTmp(sf::Mouse::getPosition(window));
    sf::Vector2f mousePosition(window.mapPixelToCoords(mousePosTmp,viewTiles));
    return mousePosition;
}


}


