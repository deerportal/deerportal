#ifndef GAME_H
#define GAME_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tilemap.h"
#include "selector.h"

namespace efc {
class Game
{
public:
    Game();
    sf::RenderWindow window;
};
}
#endif // GAME_H
