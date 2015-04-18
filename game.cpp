#include <stdlib.h>
#include <iostream>
#include "game.h"
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

    map.load(&textureTiles, sf::Vector2u(efc::TILE_SIZE, efc::TILE_SIZE), level, mapSize, mapSize);

    PlayerHud playerHud1(&textures, std::rand() % 80, &textureFaces, &textureTiles, &gameFont, 32,0);
    PlayerHud playerHud2(&textures, std::rand() % 30, &textureFaces, &textureTiles, &gameFont, 32,1);
    PlayerHud playerHud3(&textures, std::rand() % 60, &textureFaces, &textureTiles, &gameFont, 32,2);
    PlayerHud playerHud4(&textures, std::rand() % 50, &textureFaces, &textureTiles, &gameFont, 32,3);
    players[0] = playerHud1;
    players[1] = playerHud2;
    players[2] = playerHud3;
    players[3] = playerHud4;

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

Game::Game()
{
    mapSize = 16;
    std::srand (time(NULL));
    int turn = 0;

    loadAssets();



    sf::RenderWindow window(sf::VideoMode(512, 400), "EnFuCraft");
    sf::View viewGui(sf::FloatRect(00, 00, 112, 400));
    viewGui.setViewport(sf::FloatRect(0.8f,0, 1.0f, 1.0f));
    sf::View viewTiles(sf::FloatRect(00, 00, 400, 400));
    viewTiles.setViewport(sf::FloatRect(0,0, 0.8f, 1.0f));

    Selector selector(efc::TILE_SIZE);
    selector.changeColor(turn); //This is only for the test TODO: remove

    //    TileMap mapElem;
    //    if (!mapElem.load(&textureTiles, sf::Vector2u(tileSize, tileSize), levelElems, mapSize, mapSize))
    //        std::exit(1);


    initBoard();

    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();


            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "the right button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                    turn++;
                    if (turn==4)
                        turn = 0;
                    selector.changeColor(turn);
                }
            }
        }

        //
        sf::Vector2i localPositionTmp = sf::Mouse::getPosition(window);
        sf::Vector2f localPosition = window.mapPixelToCoords(localPositionTmp,viewTiles);
        if ((localPosition.x>=0) && (localPosition.y>=0) && (localPosition.x<=mapSize*efc::TILE_SIZE) && (localPosition.y<=mapSize*efc::TILE_SIZE))
        {
//            std::cout << localPosition.x << " " << localPosition.y << " " << localPosition.x / efc::TILE_SIZE << " " << localPosition.y / efc::TILE_SIZE << std::endl;
            selector.setPosition((int) (localPosition.x / efc::TILE_SIZE)*efc::TILE_SIZE, ((int) localPosition.y / efc::TILE_SIZE)*efc::TILE_SIZE);
        }

        // draw the map
        window.clear();
        window.setView(viewTiles);
        window.draw(map);
        //        window.draw(mapElem);


        for (int i=0;i<4;i++)
        {
            window.draw(players[i].elems);
        }

        window.draw(selector);

        window.setView(viewGui);
        for (int i=0;i<4;i++)
        {
            window.draw(players[i]);
        }
        window.display();
    }

}
}
