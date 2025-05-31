#include "game-assets.h"
#include "game.h"  // For access to Game class members
#include "tilemap.h"  // For ASSETS_PATH and get_full_path

namespace DP {

GameAssets::GameAssets(Game* gameInstance)
    : game(gameInstance)
{
}

GameAssets::~GameAssets()
{
    unloadAssets();
}

void GameAssets::loadAssets()
{
    loadFonts();
    loadTextures();
    loadShaders();
    loadAudio();
    initializeSprites();
    setupUI();
}

void GameAssets::loadFonts()
{
    if (!gameFont.openFromFile(get_full_path(ASSETS_PATH"fnt/metal-mania.regular.ttf")))
    {
        std::exit(1);
    }
    if (!menuFont.openFromFile(get_full_path(ASSETS_PATH"fnt/metal-macabre.regular.ttf")))
    {
        std::exit(1);
    }
}

void GameAssets::loadTextures()
{
    if (!textureBackgroundArt.loadFromFile(get_full_path(ASSETS_PATH"img/background_land.png")))
    {
        std::exit(1);
    }

    // Load and set window icon (NEW 0.8.2 FEATURE)
    sf::Image icon;
    if (icon.loadFromFile(get_full_path(ASSETS_PATH"img/deerportal.png"))) 
    {
        game->window.setIcon(icon.getSize(), icon.getPixelsPtr());
    }
}

void GameAssets::loadShaders()
{
    if (!shaderBlur.loadFromFile(get_full_path(ASSETS_PATH"shaders/blur.frag"), sf::Shader::Type::Fragment))
        std::exit(1);

    if (!shaderPixel.loadFromFile(get_full_path(ASSETS_PATH"shaders/pixelate.frag"), sf::Shader::Type::Fragment))
        std::exit(1);
        
    if (!shaderDark.loadFromFile(get_full_path(ASSETS_PATH"shaders/dark.frag"), sf::Shader::Type::Fragment))
        std::exit(1);
}

void GameAssets::loadAudio()
{
    if (!musicGame.openFromFile(get_full_path(ASSETS_PATH"audio/game.ogg")))
        std::exit(1);
        
    if (!musicMenu.openFromFile(get_full_path(ASSETS_PATH"audio/menu.ogg")))
        std::exit(1);

    if (!sfxClickBuffer.loadFromFile(get_full_path(ASSETS_PATH"audio/click.ogg")))
        std::exit(1);

    if (!sfxDoneBuffer.loadFromFile(get_full_path(ASSETS_PATH"audio/done.ogg")))
        std::exit(1);
}

void GameAssets::initializeSprites()
{
    // Initialize sprites with textures for SFML 3.0
    game->menuBackground = std::make_unique<sf::Sprite>(game->textures.textureMenu);
    game->spriteDeerGod = std::make_unique<sf::Sprite>(game->textures.textureDeerGod);
    
    // Initialize sprites that depend on loaded textures
    game->spriteBackgroundArt = std::make_unique<sf::Sprite>(textureBackgroundArt);
    game->spriteBackgroundDark = std::make_unique<sf::Sprite>(game->textures.backgroundDark);
    game->spriteLestBegin = std::make_unique<sf::Sprite>(game->textures.textureLetsBegin);
    game->spriteBigDiamond = std::make_unique<sf::Sprite>(game->textures.textureBigDiamond);

    // Initialize player sprites with texture
    for (int i = 0; i < 4; i++)
    {
        game->playersSprites[i] = std::make_unique<sf::Sprite>(textureBackgroundArt);
        game->seasons[i] = std::make_unique<sf::Sprite>(textureBackgroundArt);
        
        game->playersSprites[i]->setTextureRect(sf::IntRect({game->playersSpritesCords[i][0],
                                               game->playersSpritesCords[i][1]}, {280, 280}));
        game->playersSprites[i]->setPosition(sf::Vector2f(game->playersSpritesCords[i][0], 
                                             game->playersSpritesCords[i][1]));
    }
}

void GameAssets::setupUI()
{
    // Setup menu text
    game->menuTxt->setFont(gameFont);
    game->menuTxt->setCharacterSize(60);
    game->menuTxt->setString(game->gameTitle);
    int width = game->menuTxt->getLocalBounds().size.x;
    int height = game->menuTxt->getLocalBounds().size.y;
    game->menuTxt->setPosition(sf::Vector2f(1050-(width/2), 750-(height/2)-150));
    game->menuTxt->setFillColor(sf::Color(255, 255, 255, 85));
    
    // Setup cards deck fonts
    game->cardsDeck.setFonts(&gameFont);

    // Setup pagan holiday text
    game->paganHolidayTxt->setFont(gameFont);
    game->paganHolidayTxt->setCharacterSize(20);
    game->paganHolidayTxt->setPosition(sf::Vector2f(20, 20));

    // Setup FPS display text (NEW 0.8.2 FEATURE)
    game->textFPS->setFont(gameFont);
    game->textFPS->setCharacterSize(20);
    game->textFPS->setPosition(sf::Vector2f(0, 60));
    game->textFPS->setFillColor(sf::Color::Yellow);
    game->textFPS->setString("FPS: --");
}

void GameAssets::unloadAssets()
{
    // SFML handles automatic cleanup for textures, fonts, etc.
    // This method is here for potential future manual cleanup needs
}

void GameAssets::showMenu()
{
    playMenuMusic();
    game->currentState = Game::state_menu;
}

void GameAssets::hideMenu()
{
    stopMenuMusic();
}

void GameAssets::showGameBoard()
{
    playGameMusic();
    game->sfx.playLetsBegin();
    game->currentState = Game::state_setup_players;
}

void GameAssets::playMenuMusic()
{
    musicMenu.play();
    musicMenu.setLooping(true);
}

void GameAssets::stopMenuMusic()
{
    musicMenu.stop();
}

void GameAssets::playGameMusic()
{
    musicGame.play();
    musicGame.setLooping(true);
}

void GameAssets::stopGameMusic()
{
    musicGame.stop();
}

void GameAssets::update(sf::Time frameTime)
{
    // Update any asset-related animations or state here
    // For now, this is mainly for consistency with the interface
    // Future asset-related updates can be added here
}

} // namespace DP 