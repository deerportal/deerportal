# Plan to Fix Game State Reset on Return to Main Menu

## 1. The Problem

When a player is in the middle of a game and presses the 'Escape' key to return to the main menu, the game's state is not reset. This means that if the player starts a new game, the previous game's progress (player positions, scores, etc.) is carried over, leading to incorrect behavior.

## 2. The Cause

The issue stems from the `handleKeyboardInput` function in `src/game-input.cpp`. When the 'Escape' key is pressed, the code transitions to the main menu state but does not call the `restartGame()` function to reset the game's state.

## 3. The Solution

To fix this, I will modify `src/game-input.cpp` to call the `restartGame()` function when the 'Escape' key is pressed and the game is not already in the main menu.

### `src/game-input.cpp` Modification

In the `handleKeyboardInput` function, I will add a call to `game->restartGame()` before showing the menu.

```cpp
// src/game-input.cpp

void GameInput::handleKeyboardInput(const sf::Event& event) {
  if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == sf::Keyboard::Key::Escape) {
      // Context-aware Escape behavior
      if (game->currentState == Game::state_menu) {
        // If in menu, exit the game
        game->window.close();
      } else {
        // If in game, go back to menu and reset the game state
        game->restartGame(); // <-- Add this line
        game->stateManager->showMenu();
      }
    }
// ...
```

## 4. Verification

After implementing the change, I will manually test the fix by:

1.  Starting a new game.
2.  Playing for a few turns.
3.  Pressing the 'Escape' key to return to the main menu.
4.  Starting another new game.
5.  Verifying that the new game starts from the beginning, with all player positions and scores reset.
