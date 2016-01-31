#ifndef DATA_H
#define DATA_H
#include <string>
#include <SFML/Graphics.hpp>

namespace efc {

    extern std::string seasonsNames[4];
    enum directions {
        DIR_LEFT,
        DIR_RIGHT,
        DIR_UP,
        DIR_DOWN
    };
    extern int terrainArray[24];
    extern sf::Color playersColors[4];
    extern std::array<std::array<int,2>,256> boards;


}
#endif // DATA_H
