#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SFML/Graphics.hpp>

namespace DP {

// Forward declarations
class Game;

/*!
 * \brief GameRenderer handles all rendering and drawing functionality
 */
class GameRenderer
{
public:
    GameRenderer(Game* gameInstance);
    ~GameRenderer();

    // Main rendering
    void render(float deltaTime);

    // Specialized rendering functions
    void renderGameBoard();
    void renderUI();
    void renderEffects();
    void renderMenu();
    void renderEndGame();

    // Drawing subsystems
    void drawPlayersGui();
    void drawSquares();
    void drawBaseGame();
    void drawCharacters();

    // View and shader management
    void setView(int viewType);
    void applyShader(int shaderType, float intensity);
    void setupShaders();

    // Text and UI management
    void setTxtEndGameAmount();
    void updateFPSDisplay(float deltaTime);

private:
    Game* game;  // Reference to main game instance

    // Rendering state
    float runningCounter;
    float fpsDisplayUpdateTimer;
    float v1;  // Shader parameter

    // Helper rendering methods
    void clearBuffers();
    void drawBackground();
    void drawGameElements();
    void drawUIElements();
    void finalizeFrame(float deltaTime);

    // State-specific rendering
    void renderStateGame();
    void renderStateMenu();
    void renderStateSetup();
    void renderStateLetsBegin();
    void renderStateEndGame();
    void renderStateGuiEndRound();
    void renderStateGuiElem();

    // Shader utilities
    void updateShaderParameters();
    void applyBlurEffect(float intensity);
    void applyPixelEffect(float threshold);
};

} // namespace DP

#endif // GAME_RENDERER_H 