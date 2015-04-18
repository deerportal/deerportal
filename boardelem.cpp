#include "boardelem.h"

BoardElem::BoardElem(TextureHolder *textures,int pos, int type, const sf::Texture *textureElem)
{
//    this->textureElem = textureElem;
    this->pos = pos;
    this->textures = textures;
    this->type = type;
    //    setPosition()

    int y = (int) pos / 16;
    int x = pos % 16;




    int y_type = (int) type / 40;
    int x_type = type % 40 ;
   this->textureElem = std::move(textureElem);


    std::cout << "TYPE " << type << " x_type " << x_type << " : " << x_type * efc::TILE_SIZE << " y " << y_type << " : " << y_type * efc::TILE_SIZE << std::endl;
//std::cout << "x_type " << x_type << " : " << x_type * efc::TILE_SIZE << " y_type " << y_type << " : " << y_type * efc::TILE_SIZE << std::endl;



    setPosition(x*efc::TILE_SIZE,y*efc::TILE_SIZE);

//    setPosition(375,375);

    setTexture(myHolder.textureTiles);
    setTextureRect(sf::IntRect(x_type*efc::TILE_SIZE, y_type*efc::TILE_SIZE, efc::TILE_SIZE, efc::TILE_SIZE));


}

