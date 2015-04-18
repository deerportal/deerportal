#include "textureholder.h"

TextureHolder::TextureHolder()
{
    if (!textureTiles.loadFromFile("assets/img/zw-tilesets/_MAP.png"))
        std::exit(1);
    if (!textureFaces.loadFromFile("assets/img/faces.jpg"))
        std::exit(1);
}
