#include "textureholder.h"

TextureHolder::TextureHolder()
{
    if (!textureTiles.loadFromFile("assets/img/zw-tilesets/_MAP.png"))
        std::exit(1);
    if (!textureFaces.loadFromFile("assets/img/faces.jpg"))
        std::exit(1);
    if (!textureGui.loadFromFile("assets/img/gui.png"))
        std::exit(1);




    int defaultArray[6] = {10,2,10,0,2,2};
    int defaultFields[4] = {441, 8, 88, 328};

    for (int i: defaultFields)
    {
            std::map<int, int> params;
            params.insert({0, 10});
            params.insert({1, 2});
            params.insert({2, 10});
            params.insert({3, 0});
            params.insert({4, 10});
            params.insert({5, 0});

            tilesDescription.insert({i, params});

    }


//    tilesDescription.insert({441, &defaultArray});
//    tilesDescription.insert({8, &defaultArray});
//    tilesDescription.insert({88, &defaultArray});
//    tilesDescription.insert({328, &defaultArray});


}
