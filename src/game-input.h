#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace DP {

// Forward declarations
class Game;

/*!
 * \brief GameInput handles all input processing and event management
 */
class GameInput {
public:
  GameInput(Game* gameInstance);
  ~GameInput();

  // Main event processing
  void processEvents(sf::RenderWindow& window);

  // Input handling methods
  void handleLeftClick(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);
  void handleKeyboardInput(const sf::Event& event);
  void handleMouseInput(const sf::Event& event);

  // Mouse position and selector management
  void updateSelector(sf::Vector2f localPosition);
  sf::Vector2f getMousePosition(sf::RenderWindow& window) const;

  // State-specific input validation
  bool isInputAllowed() const;
  void setInputMode(int gameState);

private:
  Game* game; // Reference to main game instance

  // Input state tracking
  sf::Vector2f lastMousePosition;
  bool mouseOverBoard;
  int currentInputMode;

  // State-specific input handlers
  void processGameInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);
  void processSetupInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);
  void processRollDiceInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);
  void processMenuInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);
  void processGuiEndRoundInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);
  void processLetsBeginInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);
  void processEndGameInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);

  // Helper methods
  void validateMousePosition(sf::Vector2f position);

  // Mouse coordinate conversion
  sf::Vector2f mapToTileCoords(sf::Vector2f screenPos, sf::RenderWindow& window);
  sf::Vector2f mapToGuiCoords(sf::Vector2f screenPos, sf::RenderWindow& window);
  int convertToTileIndex(sf::Vector2f tileCoords);
};

} // namespace DP

#endif // GAME_INPUT_H