#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "textureholder.h"

namespace DP {

// Forward declarations
class Game;

/*!
 * \brief GameAssets handles all asset loading, management, and menu system functionality
 */
class GameAssets
{
public:
    GameAssets(Game* gameInstance);
    ~GameAssets();

    // Asset loading and management
    void loadAssets();
    void unloadAssets();

    // Menu system
    void showMenu();
    void hideMenu();
    void showGameBoard();

    // Update functionality
    void update(sf::Time frameTime);

    // Asset access
    const sf::Font& getGameFont() const { return gameFont; }
    const sf::Font& getMenuFont() const { return menuFont; }
    
    // Audio control
    void playMenuMusic();
    void stopMenuMusic();
    void playGameMusic();
    void stopGameMusic();

private:
    Game* game;  // Reference to main game instance

    // Fonts
    sf::Font gameFont;
    sf::Font menuFont;

    // Textures
    sf::Texture textureBackgroundArt;

    // Audio
    sf::Music musicGame;
    sf::Music musicMenu;
    sf::SoundBuffer sfxClickBuffer;
    sf::SoundBuffer sfxDoneBuffer;

    // Shaders
    sf::Shader shaderBlur;
    sf::Shader shaderPixel;
    sf::Shader shaderDark;

    // Helper methods
    void loadFonts();
    void loadTextures();
    void loadShaders();
    void loadAudio();
    void initializeSprites();
    void setupUI();
};

} // namespace DP

#endif // GAME_ASSETS_H 