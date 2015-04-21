#include <stdlib.h>
#include <iostream>
#include "game.h"
#include "guiwindow.h"
#include <time.h>       /* time */

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
    currentNeighbours = players[0].getNeighbours();
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
}

Game::Game():
    window(sf::VideoMode(512, 400), "EnFuCraft"),
    viewTiles(sf::FloatRect(00, 00, 400, 400)),
    turn(0)
{
    Hover hover;
    GuiWindow guiWindow(&textures);
    GuiChooseBuilding guiSelectBuilding(&textures);
    currentState = state_init;
    std::srand (time(NULL));
    loadAssets();
    sf::View viewGui(sf::FloatRect(00, 00, 112, 400));
    viewGui.setViewport(sf::FloatRect(0.8f,0, 1.0f, 1.0f));
    sf::View viewTiles(sf::FloatRect(00, 00, 400, 400));
    viewTiles.setViewport(sf::FloatRect(0,0, 0.8f, 1.0f));
    Selector selector(efc::TILE_SIZE);
    selector.changeColor(turn); //This is only for the test TODO: remove
    initBoard();
    currentState = state_game;

    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        sf::Vector2i localPositionTmp = sf::Mouse::getPosition(window);
        sf::Vector2f localPosition = window.mapPixelToCoords(localPositionTmp,viewTiles);

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


        while (window.pollEvent(event))
        {

            int mousePosX = (int)localPosition.x / efc::TILE_SIZE;
            int mousePosY = (int)localPosition.y / efc::TILE_SIZE;
            int mousePos = efc::transCords(sf::Vector2i(mousePosX, mousePosY));
            if(event.type == sf::Event::Closed)
                window.close();
            if (currentNeighbours.find(mousePos) != currentNeighbours.end())
            {


            }

            if (event.type == sf::Event::MouseButtonReleased)
            {

                sf::Vector2i mTP = sf::Mouse::getPosition(window);

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (currentState==state_game)
                    {
                        if (currentNeighbours.find(mousePos) != currentNeighbours.end())
                        {
                            std::cout << "SUPER" << std::endl;
                            if (!guiSelectBuilding.active)
                            {
                                selectedPos = mousePos;
                                guiSelectBuilding.setPosition(hover_x, hover_y);
                                guiSelectBuilding.active = true;
                                currentState = state_gui_elem;
                            }
                            break;
                        }
                        turn++;
                        if (turn==4)
                            turn = 0;
                        selector.changeColor(turn);
                        for (int i=0;i<4;i++)
                        {
                            if (i==turn)
                            {
                                players[i].setActive(true);
                                if (currentNeighbours.find(mousePos) != currentNeighbours.end())
                                {
                                    std::cout << "SUPER" << std::endl;
                                }
                                currentNeighbours = players[i].getNeighbours();
                            }
                            else
                                players[i].setActive(false);
                        }
                    }
                    if (currentState==state_gui_elem)
                    {
                        std::string result = guiSelectBuilding.getElem(localPosition);
                        command(result);
                    }


                }
            }
        }
        if ((localPosition.x>=0) && (localPosition.y>=0) && (localPosition.x<=efc::BOARD_SIZE*efc::TILE_SIZE) && (localPosition.y<=efc::BOARD_SIZE*efc::TILE_SIZE))
        {
            selector.setPosition((int) (localPosition.x / efc::TILE_SIZE)*efc::TILE_SIZE, ((int) localPosition.y / efc::TILE_SIZE)*efc::TILE_SIZE);
        }


        window.clear();

        if ((currentState==state_game) || (currentState==state_gui_elem))
        {
            window.setView(viewTiles);
            window.draw(map);
            for (int i=0;i<4;i++)
            {
                window.draw(players[i].elems);
            }
            window.draw(selector);
            window.setView(viewGui);
            drawPlayersGui();

        }

        window.setView(viewTiles);
        window.draw(guiSelectBuilding);
        window.display();
    }
}

void Game::drawPlayersGui(){
    for (int i=0;i<4;i++)
    {
        window.draw(players[i]);
    }
}

void Game::command(std::string command){
    std::cout << "RUNNING:" + command  << std::endl;
    if (command=="close")
        currentState=state_game;
    if (command.find("build_")==0)
    {
        int buildingType = std::stoi(command.substr(6));
        std::cout << "building:" + buildingType << " at " << selectedPos << std::endl;
        players[turn].addElem(selectedPos, buildingType);
        currentState=state_game;
    }

}

sf::Vector2f Game::getMousePos(){
    sf::Vector2i mousePosTmp(sf::Mouse::getPosition(window));
    sf::Vector2f mousePosition(window.mapPixelToCoords(mousePosTmp,viewTiles));
    return mousePosition;
}


}

