#include <iostream>
#include "textureholder.h"

namespace efc {

std::set<int> getTerrainSet() {
    std::set<int> terrain;
    for (int i: efc::terrainArray)
    {
        terrain.insert(i);
    }
    return terrain;
}

}
TextureHolder::TextureHolder()
{
    //     "stop", "card", "diamond", "diamond x 2"

    std::string cardsImages[4][4]  = {
        {"card-water-stop.small.png", "card-water-remove-card.small.png","card-water-diam.small.png",  "card-water-2-diam.small.png" },
        {"card-earth-stop.small.png", "card-earth-remove-card.small.png","card-earth-diam.small.png",  "card-earth-2-diam.small.png"},
        {"card-fire-stop.small.png",   "card-fire-remove-card.small.png","card-fire-diam.small.png",   "card-fire-2-diam.small.png"},
        {"card-air-stop.small.png",    "card-air-remove-card.small.png", "card-air-diam.small.png",    "card-air-2-diam.small.png" }


    };

    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            if(!cardsTextures[i][j].loadFromFile("assets/img/cards/"+cardsImages[i][j]))
            {
                std::cout << "assets/img/cards/" << cardsImages[i][j];
                std::exit(1);
            } else {
                std::cout << "assets/img/cards/" << cardsImages[i][j] << " ok " << i << " " <<  j << " " << std::endl;
            }
        }
    }



    if (!textureCardBases[0].loadFromFile("assets/img/card-water-2-diam_m.png"))
        std::exit(1);

    if (!textureCardBases[1].loadFromFile("assets/img/card-earth-2-diam_m.png"))
        std::exit(1);

    if (!textureCardBases[2].loadFromFile("assets/img/card-fire-2-diam_m.png"))
        std::exit(1);

    if (!textureCardBases[3].loadFromFile("assets/img/card-air-2-diam_m.png"))
        std::exit(1);


//    if (!textureGameBackground.loadFromFile("assets/img/game-ackground.png"))
//        std::exit(1);

//    if (!textureTiles.loadFromFile("assets/img/zw-tilesets/_MAP.png"))
//        std::exit(1);
//    if (!textureFaces.loadFromFile("assets/img/faces.jpg"))
//        std::exit(1);
//    if (!textureGui.loadFromFile("assets/img/gui.png"))
//        std::exit(1);

//    if (!textureSymbols.loadFromFile("assets/img/symbols.png"))
//        std::exit(1);
//    if (!textureSeasons.loadFromFile("assets/img/seasons.png"))
//        std::exit(1);

    if (!textureCharacters.loadFromFile("assets/img/characters.png"))
        std::exit(1);

    if (!backgroundDark.loadFromFile("assets/img/background_dark.png"))
        std::exit(1);
    if (!textureBoardDiamond.loadFromFile("assets/img/board_diamonds.png"))
        std::exit(1);

//    if (!textureMenu.loadFromFile("assets/img/background_new.png"))
//        std::exit(1);

    if (!textureLetsBegin.loadFromFile("assets/img/letsbegin.png"))
        std::exit(1);

    int defaultArray[5][8] = {
        //Cash   Food    Energy  Faith
        {10, 2,  0,  0,  0,  0,  0,  0}, // base
        {10, 1,  2,  0,  0,  5,  0,  0}, // windmill
        {15, 0,  0,  2,  2,  0,  0,  0}, // granary
        {20, 5,  4,  0,  4,  0,  0,  0}, // marketplace
        {5,  0,  2,  0,  0,  0,  0,  2} // monasterium
    };
    int defaultFields[5] = {443, 651, 442, 585, 1100};


    /*
     * Array with description of the field
     * global rule = even indexes are price, odd - monthly cost
     * [0] - price in cash
     * [1] - monthly cash cost
     * [2] - price - in food
     * [3] - monthly food cost
     * [4] - price in energy
     * [5] - monthly energy cost
     * [6] - price in faith
     * [7] - monthly cost in faith
     *
     */

    int counter = 0;
    for (int i: defaultFields)
    {
            std::map<int, int> params;
            for (int j=0;j<8;j++)
            {
                params.insert({j, defaultArray[counter][j]});
            }
//            params.insert({0, 10});
//            params.insert({1, 2});
//            params.insert({2, 10});
//            params.insert({3, 0});
//            params.insert({4, 10});
//            params.insert({5, 0});
//            params.insert({6, 0});
//            params.insert({7, 0});
            tilesDescription.insert({i, params});
            counter++;
    };
    tilesTxt.insert({443, "Your base."});
    tilesTxt.insert({651, "Windmill, produces energy.\nEnergy is a basic resource in a game,\nneeded by other buildings to running them."});
    tilesTxt.insert({442, "Granary, food storehouse.\nFood gives your people ability to live.\nWithout the food your people will face\na death by starvation."});
    tilesTxt.insert({585, "Marketplace, generates cash.\nProvides your tribe trading area.\nToday's offer - dog's bone."});
    tilesTxt.insert({1100, "Monasterium, increase your faith.\nAs we all know, our world is being ruled\nby four ancient gods..."});





}
