#include <stdlib.h>
#include <iostream>
#include "game.h"

#include "playerhud.h"


namespace efc {
Game::Game()
{
    int tileSize = 25;
    int mapSize = 16;

    sf::Texture textureFaces;


    if (!textureFaces.loadFromFile("assets/img/faces.jpg"))
                std::exit(1);

    sf::RenderWindow window(sf::VideoMode(512, 400), "Tilemap");
    // Grass tile starts at 342 and has 11 tiles
    int level[256];
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
        int grass = rand() % 11;
        level[i] = 342 + grass;
    }
    sf::View viewGui(sf::FloatRect(00, 00, 112, 400));
    viewGui.setViewport(sf::FloatRect(0.8f,0, 1.0f, 1.0f));


    sf::View viewTiles(sf::FloatRect(00, 00, 400, 400));
    viewTiles.setViewport(sf::FloatRect(0,0, 0.8f, 1.0f));


    // create the tilemap from the level definition
    TileMap map;
        Selector selector(tileSize);

        int faceNumber = std::rand() % 80;
//        faceNumber = 1;
        PlayerHud playerHud1(std::rand() % 80, &textureFaces, 32,0);
    PlayerHud playerHud2(std::rand() % 80, &textureFaces, 32,1);
    PlayerHud playerHud3(std::rand() % 80, &textureFaces, 32,2);
    PlayerHud playerHud4(std::rand() % 80, &textureFaces, 32,3);
    if (!map.load("assets/img/zw-tilesets/_MAP.png", sf::Vector2u(tileSize, tileSize), level, mapSize, mapSize))
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
