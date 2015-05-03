#include "game.h"

namespace efc {

void Game::initBoard()
{
    // Grass tile starts at 342 and has 11 tiles
    //    int level[256];
    //    int level[256];

    // Structure of the board
    /*
    {
        342, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        80, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3 }
    */

    //    Fill the array
    for (int i=0;i<256;i++)
    {
        int grass = (rand() % 10) + 1;
        level[i] = 342 + grass;
        level[i] = level[i];
    }


    //    level[0] = 441;
    //    level[240] = 0;
    //    level[255] = 0;
    //    level[15] = 0;

    level[8] = 813;
    level[24] = 803;
    level[40] = 803;
    level[56] = 803;
    level[72] = 803;

    level[88] = 801;

    level[167] = 809;
    level[183] = 803;
    level[199] = 803;
    level[215] = 803;
    level[231] = 803;
    level[247] = 812;

    level[112] = 811;
    for (int i=113;i<117;i++)
        level[i] = 816;
    level[117] = 815;

    level[138] = 800;
    for (int i=139;i<143;i++)
        level[i] = 816;
    level[143] = 814;

    map.load(&textures, sf::Vector2u(efc::TILE_SIZE, efc::TILE_SIZE), level, efc::BOARD_SIZE, efc::BOARD_SIZE);

    PlayerHud playerHud1(&textures, std::rand() % 80, &gameFont, 32,0);
    PlayerHud playerHud2(&textures, std::rand() % 30, &gameFont, 32,1);
    PlayerHud playerHud3(&textures, std::rand() % 60, &gameFont, 32,2);
    PlayerHud playerHud4(&textures, std::rand() % 50, &gameFont, 32,3);
    players[0] = playerHud1;
    players[1] = playerHud2;
    players[2] = playerHud3;
    players[3] = playerHud4;




    players[0].setActive(true);
    setCurrentNeighbours();
}


void Game::setCurrentNeighbours ()
{
    currentNeighbours = players[turn].getNeighbours();
}

void Game::loadAssets()
{
    if (!textureFaces.loadFromFile("assets/img/faces.jpg"))
        std::exit(1);

    if (!textureTiles.loadFromFile("assets/img/zw-tilesets/_MAP.png"))
        std::exit(1);

    if (!gameFont.loadFromFile("assets/fnt/8bitOperatorPlus-Regular.ttf"))
    {
        std::exit(1);
    }
    if (!menuFont.loadFromFile("assets/fnt/MorrisJensonInitialen.ttf"))
    {
        std::exit(1);
    }



    menuTxt.setFont(menuFont);
    menuTxt.setCharacterSize(120);
    menuTxt.setString(gameTitle);

    menuTxt.setColor(sf::Color(55, 255, 35, 85));

    int width = menuTxt.getLocalBounds().width;
    int height = menuTxt.getLocalBounds().height;

    menuTxt.setPosition(400-(width/2),300-(height/2)-150);
    menuTxt.setScale(0.5, 0.5);


}


void Game::showMenu()
{
    menuBackground.setTexture(textures.textureMenu);
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
    musicGame.play();
    musicGame.setLoop(true);
    currentState = state_game;
}

void Game::hideGameBoard()
{
    musicGame.play();

}

Game::Game():
    window(sf::VideoMode(800, 600), "Pagan Board"),
    viewTiles(sf::FloatRect(00, 00, 600, 522)),
    viewFull(sf::FloatRect(00, 00, 800, 600)),
    viewGui(sf::FloatRect(00, 00, 112, 522)),
    selector(efc::TILE_SIZE),
    guiSelectBuilding(&textures),
    turn(0),
    gameTitle(" PAgAN\nBOaRD "),
    roundDice(players),
    roundNumber(1),
    guiRoundDice(&textures)
{

    if (!textureBackground.loadFromFile("assets/img/background.png"))
         std::exit(1);

    spriteBackground.setTexture(textureBackground);

    guiRoundDice.active = true;
    showPlayerBoardElems = false;
    if (!musicGame.openFromFile("assets/audio/game.ogg"))
        std::exit(1);
    if (!musicBackground.openFromFile("assets/audio/wind.ogg"))
        std::exit(1);
    if (!musicMenu.openFromFile("assets/audio/menu.ogg"))
        std::exit(1);


    if (!sfxClickBuffer.loadFromFile("assets/audio/click.ogg"))
        std::exit(1);
    sfxClick.setBuffer(sfxClickBuffer);
    window.setVerticalSyncEnabled(true);
    Hover hover;
    GuiWindow guiWindow(&textures);



    std::srand (time(NULL));
    loadAssets();
    viewTiles.setViewport(sf::FloatRect(0.04f,0.066f, 1.0f, 1.14f));
    viewGui.setViewport(sf::FloatRect(0.71f,0.066f, 1.04f, 1.14f));

    selector.changeColor(turn); //This is only for the test TODO: remove
    initBoard();
    showMenu();


    // run the main loop
    while (window.isOpen())
    {

        std::string resultCommand = "";



        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i localPositionTmp = sf::Mouse::getPosition(window);
            sf::Vector2f localPosition = window.mapPixelToCoords(localPositionTmp,viewTiles);
            sf::Vector2f localPositionGui = window.mapPixelToCoords(localPositionTmp,viewGui);

            int mousePosX = (int)localPosition.x / efc::TILE_SIZE;
            int mousePosY = (int)localPosition.y / efc::TILE_SIZE;
            int mousePos = efc::transCords(sf::Vector2i(mousePosX, mousePosY));
            if(event.type == sf::Event::Closed)
                window.close();

            if (currentState==state_gui_elem)
            {
                resultCommand = guiSelectBuilding.getElem(localPosition);


                if (resultCommand.find("elem_")==0)
                    command(resultCommand);
                else
                    command("hide_gui_elem_description");



            }



            if (currentState==state_game)
            {


                if ((localPosition.x>400) || (localPosition.x<0)  || (localPosition.y>400) || (localPosition.y<0))
                {
                    showPlayerBoardElems = false;
                    players[turn].elems.displayNeighbours = false;
                } else {
                    showPlayerBoardElems = true;
                    players[turn].elems.displayNeighbours = true;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {


                    if (currentState==state_game)
                    {


                        resultCommand = players[turn].getElem(localPositionGui);
                        command(resultCommand);

                        if (currentNeighbours.find(mousePos) != currentNeighbours.end())
                        {

                            if ((!guiSelectBuilding.active) && (showPlayerBoardElems))
                            {
                                float hover_x =localPosition.x;
                                float hover_y = localPosition.y;
                                if (localPosition.y > 290)
                                    hover_y = hover_y - 100;
                                if (localPosition.x > 240)
                                    hover_x = hover_x - 150;
                                if (hover_x>250)
                                    hover_x = 249.0f;
                                if (hover_x<0)
                                    hover_x = 1.0f;
                                if (hover_y>300)
                                    hover_y = 299.0f;
                                if (hover_y<0)
                                    hover_y = 1.0f;

                                selectedPos = mousePos;
                                guiSelectBuilding.setPosition(hover_x, hover_y);
                                guiSelectBuilding.active = true;
                                sfxClick.play();
                                currentState = state_gui_elem;
                            }
                            break;
                        }





                        //                        nextPlayer();
                    }

                    if (currentState==state_gui_elem)
                    {
                        resultCommand = guiSelectBuilding.getElem(localPosition);
                        if (resultCommand.find("elem_")==0)
                        {
                            std::string resultCommandWrapped = "build_" + resultCommand;
                            command(resultCommandWrapped);
                        } else if (resultCommand.find("close_gui")==0)
                        {        command(resultCommand);}
                    }
                    if (currentState==state_menu)
                    {
                        hideMenu();
                        showGameBoard();


                    }

                    if (currentState==state_gui_end_round)
                    {
                        resultCommand = guiRoundDice.getElem(localPosition);
                            command(resultCommand);


                    }


                }
            }
            if ((localPosition.x>=0) && (localPosition.y>=0) && (localPosition.x<=efc::BOARD_SIZE*efc::TILE_SIZE) && (localPosition.y<=efc::BOARD_SIZE*efc::TILE_SIZE))
            {
                selector.setPosition((int) (localPosition.x / efc::TILE_SIZE)*efc::TILE_SIZE, ((int) localPosition.y / efc::TILE_SIZE)*efc::TILE_SIZE);
            }
        }





        render();
    }
}
void Game::nextRound() {
    turn = 0;
    std::string result = roundDice.drawRound();
    roundNumber += 1;
    std::cout << "END OF ROUND " << roundNumber << " " << result << std::endl;
    command(result);
}

void Game::nextPlayer(){
    players[turn].updatePlayer();
    turn++;

    if (turn==4)
    {
        nextRound();
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
}

void Game::drawPlayersGui(){
    for (int i=0;i<4;i++)
    {
        window.draw(players[i]);
    }
}

void Game::drawSquares() {
    if (showPlayerBoardElems)
    {

        window.draw(selector);
    }

}



void Game::update()
{

}

void Game::drawBaseGame()
{
    window.setView(viewTiles);
    window.draw(map);
    for (int i=0;i<4;i++)
    {
        window.draw(players[i].elems);
    }
    drawSquares();
    window.setView(viewGui);
    drawPlayersGui();
    window.setView(viewTiles);
}

void Game::render()
{
    window.clear();



    if (currentState==state_game)
    {
        drawBaseGame();

    } else if (currentState==state_gui_elem) {
        drawBaseGame();
        window.draw(guiSelectBuilding);
    }  else if (currentState==state_menu) {

        window.draw(menuBackground);
        window.draw(menuTxt);


    } else if (currentState==state_gui_end_round){
        drawBaseGame();
        window.draw(guiRoundDice);
    }
    window.setView(viewFull);
    window.draw(spriteBackground);

    window.display();
}

void Game::command(std::string command){
        std::cout << command << std::endl;
    if (command=="close_gui")
    {
        guiSelectBuilding.active = false;
        currentState=state_game;
        sfxClick.play();
    }

    if (command=="hide_gui_elem_description")
    {
        if (currentState==state_gui_elem) {
            guiSelectBuilding.descriptionActive = false;

        }
    }

    if (command.find("end_of_round")==0)
    {
        std::string subResult = command.substr(13);
        std::cout << "SUB RESULT " << subResult << std::endl;
        guiRoundDice.active = true;
        guiRoundDice.setTitle(subResult);
        currentState = state_gui_end_round;
    }

    if (command.find("elem_")==0)
    {
        if (currentState==state_gui_elem)
        {
            int buildingType = std::stoi(command.substr(5));
            int cashUpd  = textures.tilesDescription[buildingType][0];
            int foodUpd  = textures.tilesDescription[buildingType][2];
            int enrgUpd  = textures.tilesDescription[buildingType][4];
            std::string descTxt = textures.tilesTxt[buildingType];
            guiSelectBuilding.setDescriptionTxt("Cash:" + std::to_string(cashUpd) +"\n"+descTxt);
            guiSelectBuilding.descriptionActive = true;
        }
    }

    if (command=="end_turn")
        nextPlayer();
    if (command.find("build_")==0)
    {
        int buildingType = std::stoi(command.substr(11));
        bool purchaseResult = players[turn].addElem(selectedPos, buildingType);
        if (purchaseResult)
        {
            currentState = state_game;
            setCurrentNeighbours();
            guiSelectBuilding.active = false;
            sfxClick.play();



        }
    }
}

sf::Vector2f Game::getMousePos(){
    sf::Vector2i mousePosTmp(sf::Mouse::getPosition(window));
    sf::Vector2f mousePosition(window.mapPixelToCoords(mousePosTmp,viewTiles));
    return mousePosition;
}


}

