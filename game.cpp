#include <stdlib.h>
#include <iostream>
#include "game.h"
#include <time.h>       /* time */

#include "playerhud.h"


namespace efc {
Game::Game()
{
    int tileSize = 25;
    int mapSize = 16;
    std::srand (time(NULL));

    int turn = 0;



    sf::Texture textureFaces;


    if (!textureFaces.loadFromFile("assets/img/faces.jpg"))
        std::exit(1);

    sf::Font gameFont;
    if (!gameFont.loadFromFile("assets/fnt/8bitOperatorPlus-Regular.ttf"))
    {
        std::exit(1);
    }


    sf::RenderWindow window(sf::VideoMode(512, 400), "EnFuCraft");
    // Grass tile starts at 342 and has 11 tiles
    int level[256];
    int levelElems[256];
    /* =
    {
        342, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        80, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
        342, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        80, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };*/


    //    Fill the array
    for (int i=0;i<256;i++)
    {
        int grass = (rand() % 10) + 1;
        level[i] = 342 + grass;
        levelElems[i] = level[i];
    }


    sf::View viewGui(sf::FloatRect(00, 00, 112, 400));
    viewGui.setViewport(sf::FloatRect(0.8f,0, 1.0f, 1.0f));


    sf::View viewTiles(sf::FloatRect(00, 00, 400, 400));
    viewTiles.setViewport(sf::FloatRect(0,0, 0.8f, 1.0f));


    // create the tilemap from the level definition
    TileMap map;
    Selector selector(tileSize);
    TileMap mapElem;

    selector.changeColor(turn);
    PlayerHud playerHud1(std::rand() % 80, &textureFaces, &gameFont, 32,0);
    PlayerHud playerHud2(std::rand() % 30, &textureFaces, &gameFont, 32,1);
    PlayerHud playerHud3(std::rand() % 60, &textureFaces, &gameFont, 32,2);
    PlayerHud playerHud4(std::rand() % 50, &textureFaces, &gameFont, 32,3);
    PlayerHud players[4] = {playerHud1, playerHud2, playerHud3, playerHud4};


    if (!map.load("assets/img/zw-tilesets/_MAP.png", sf::Vector2u(tileSize, tileSize), level, mapSize, mapSize))
        std::exit(1);


    levelElems[0] = 0;
    levelElems[15] = 0;
    levelElems[8] = 813;
    levelElems[24] = 803;
    levelElems[40] = 803;
    levelElems[56] = 803;
    levelElems[72] = 803;

    levelElems[88] = 801;
//    levelElems[104] = 80;
//    levelElems[120] = 803;
//    levelElems[135] = 803;
//    levelElems[151] = 803;
//    levelElems[161] = 803;
    levelElems[167] = 809;
    levelElems[183] = 803;
    levelElems[199] = 803;
    levelElems[215] = 803;
    levelElems[231] = 803;
    levelElems[247] = 812;

    levelElems[112] = 811;
    for (int i=113;i<117;i++)
        levelElems[i] = 816;
    levelElems[117] = 815;

    levelElems[138] = 800;
    for (int i=139;i<143;i++)
        levelElems[i] = 816;
    levelElems[143] = 814;

    levelElems[240] = 0;
    levelElems[255] = 0;



    if (!mapElem.load("assets/img/zw-tilesets/_MAP.png", sf::Vector2u(tileSize, tileSize), levelElems, mapSize, mapSize))
        std::exit(1);


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
        sf::Vector2i localPositionTmp = sf::Mouse::getPosition(window);
        sf::Vector2f localPosition = window.mapPixelToCoords(localPositionTmp,viewTiles);



        if ((localPosition.x>=0) && (localPosition.y>=0) && (localPosition.x<=mapSize*tileSize) && (localPosition.y<=mapSize*tileSize))
        {
            std::cout << localPosition.x << " " << localPosition.y << " " << localPosition.x / tileSize << " " << localPosition.y / tileSize << std::endl;
            selector.setPosition((int) (localPosition.x / tileSize)*tileSize, ((int) localPosition.y / tileSize)*tileSize);
        }



        // draw the map
        window.clear();


        window.setView(viewTiles);

        window.draw(map);
        window.draw(mapElem);

        window.draw(selector);

        window.setView(viewGui);
        window.draw(playerHud1);
        window.draw(playerHud2);
        window.draw(playerHud3);
        window.draw(playerHud4);
        window.display();







    }

}
}
