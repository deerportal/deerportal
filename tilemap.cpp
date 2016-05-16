#include "tilemap.h"
namespace efc {

/*!
 * Returns sf::Vector2i cords taking int board position as input.
 */
sf::Vector2i transPosition(int pos) {
    int x = (int) pos % efc::BOARD_SIZE;
    int y = (int) pos / efc::BOARD_SIZE;
    sf::Vector2i cords(x,y);

    return cords;
}

sf::Vector2i transTilePosition(int pos) {
    int x = (int) pos % efc::TILE_BOARD_SIZE;
    int y = (int) pos / efc::TILE_BOARD_SIZE;
    sf::Vector2i cords(x,y);

    return cords;
}

/*!
 * Returns int cords taking screen sf::Vector2f position
 * as the input.
 */
sf::Vector2i getCords(sf::Vector2f position){
    int x = position.x/efc::TILE_BOARD_SIZE;
    int y = position.y/efc::TILE_BOARD_SIZE;
    return sf::Vector2i(x, y);
}

/*!
 * Returns screen sf::Vector2f pos taking sf::Vector2i cords
 * as the input.
 */
sf::Vector2f getScreenPos(sf::Vector2i cords){
    float x = cords.x * efc::TILE_SIZE;
    float y = cords.y * efc::TILE_SIZE;
    return sf::Vector2f(x, y);
}


/*!
 * Returns int board position (1-256, 16x16) taking
 * sf::Vector2i cords as the input.
 */
int transCords(sf::Vector2i cords) {
    //    std::cout << cords.x << " " << cords.y << " " << std::endl;
    int pos = (cords.y * efc::BOARD_SIZE)+cords.x;
    return pos;
}

sf::IntRect transPosIntoRect(int pos)
{
    sf::Vector2i cords = efc::transTilePosition(pos);
    sf::IntRect posRect((int)cords.x*efc::TILE_SIZE, (int)cords.y*efc::TILE_SIZE, efc::TILE_SIZE, efc::TILE_SIZE);
    //    std::cout << "debug transPosIntoRect " << pos << " "<<  cords.x << " " << cords.y << " " << posRect.left << " " << posRect.top << std::endl;
    return posRect;
}

std::set<int>  getNeighbours(int pos) {
    sf::Vector2i cords = efc::transPosition(pos);



    std::vector<int> neighbours;
    std::set<int> neighboursSet;

    std::set<int> terrain = efc::getTerrainSet();


    int value = -1;

    if (cords.x>0)
    {
        neighbours.push_back(pos-1);
        //        neighboursSet.insert(pos-1);
        value = pos-1;
        if ((value!=-1) && (terrain.count(value)==0))
            neighboursSet.insert(value);


    }
    if (cords.x<efc::BOARD_SIZE-1)
    {
        neighbours.push_back(pos+1);
        //        neighboursSet.insert(pos+1);
        value = pos+1;
        if ((value!=-1) && (terrain.count(value)==0))
            neighboursSet.insert(value);
    }

    if (cords.y>0)
    {
        neighbours.push_back(pos-efc::BOARD_SIZE);
        //        neighboursSet.insert(pos-efc::BOARD_SIZE);
        value = pos-efc::BOARD_SIZE;
        if ((value!=-1) && (terrain.count(value)==0))
            neighboursSet.insert(value);
    }
    if (cords.y<efc::BOARD_SIZE)
    {
        neighbours.push_back(pos+efc::BOARD_SIZE);
        //        neighboursSet.insert(pos+efc::BOARD_SIZE);
        value = pos+efc::BOARD_SIZE;
        if ((value!=-1) && (terrain.count(value)==0))
            neighboursSet.insert(value);
    }




        return neighboursSet;
}
}

TileMap::TileMap()
{
    textures = nullptr;
    m_tileset = nullptr;
}

//bool TileMap::load(TextureHolder *textures, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
//{
//    this->m_tileset = &textures->textureTiles;
//    // resize the vertex array to fit the level size
//    m_vertices.setPrimitiveType(sf::Quads);
//    m_vertices.resize(width * height * 4);

//    // populate the vertex array, with one quad per tile
//    for (unsigned int i = 0; i < width; ++i)
//        for (unsigned int j = 0; j < height; ++j)
//        {
//            // get the current tile number
//            int tileNumber = tiles[i + j * width];

//            // find its position in the tileset texture
//            int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
//            int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

//            // get a pointer to the current tile's quad
//            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

//            // define its 4 corners
//            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
//            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
//            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
//            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

//            // define its 4 texture coordinates
//            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
//            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
//            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
//            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
//        }
//    return true;
//}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();
    // apply the tileset texture
    states.texture = m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

sf::VertexArray m_vertices;
sf::Texture m_tileset;
