# GEMINI PLAN: Animated Board Initialization

## 1. Conquest Goals (Executive Summary)

This sacred plan, forged from the wisdom of the `FINAL-SONNET.md` and a fresh survey of the codebase, outlines the glorious conquest of implementing a smooth, animated board setup. We shall make the diamonds and runes dance from the four corners of the world (quadrants) onto the game board. This will be achieved using the existing powers of VertexArray optimization, state management, and our animation sagas, ensuring every piece lands with the precision of a thrown axe.

## 2. The Spoils of War (Core Technical Requirements)

### Diamond Placement & Runes of Position
- **The Horde**: 112 `BoardDiamond` objects are our warriors (`DP::diamondsNumber = 112`).
- **Warbands**: Each of the four players commands a warband of 28 diamonds.
- **Player Banners (Index Correction)**: The banners are raised thusly: Player 0 (diamonds 0-27), Player 1 (28-55), Player 3 (56-83), and Player 2 (84-111). Note the cunning swap of Players 2 and 3!
- **Charting the Land (Position Calculation)**: The final resting place of each diamond is decreed by `DP::transPosition(boardPosition)` which yields grid coordinates, followed by `DP::getScreenPos(coords)` to get screen position. To this, we add a global offset of (202, 76) and a centering blessing of (2.4f, 2.4f).
- **The Battlefield (Board Grid)**: A 16x16 grid, divided into four quadrants: Q0 (top-left), Q1 (top-right), Q2 (bottom-left), Q3 (bottom-right).

### Weaving the Fates (State System Integration)
- **The Current Saga**: `state_setup_players` leads to `state_lets_begin`.
- **A New Chapter**: We shall insert `state_board_animation` between these two chapters.
- **The Call to Adventure (Trigger)**: The animation begins its tale after `GameStateManager::showGameBoard()` calls `restartGame()`, once the diamonds' final positions are known.
- **The Storytellers**: `GameStateManager`, `GameRenderer`, and `GameInput` must all learn this new part of the saga.

### Shield Wall Against Chaos (Fullscreen & SFML 3.0.1 Alignment)
- **World Tree Mapping (WindowManager)**: We will use `window.mapPixelToCoords()` to ensure our warriors find their places even when the world is stretched to fullscreen. This is in line with SFML 3.x's view and coordinate handling.
- **Scaling the World**: `WindowManager::updateView()` will be honored for its power to transform coordinates correctly.
- **The All-Seeing Eye (Render Texture)**: Our animation must be visible through the existing `renderTexture` pipeline.

## 3. The War Council (Architecture & Implementation)

### Our Chieftains (Core Classes)

These new classes shall lead our charge. They will be created in the `src/` directory.

#### `BoardSpawnRegions` (src/board-spawn-regions.h & .cpp)
This class shall be the loremaster, knowing where each diamond begins its journey.
```cpp
// src/board-spawn-regions.h
#pragma once
#include <SFML/Graphics.hpp>

class BoardSpawnRegions {
public:
    int getQuadrantForDiamond(int diamondIndex);
    sf::Vector2f getSpawnPoint(int quadrant, const sf::RenderWindow& window);
    sf::Vector2f getQuadrantCenter(int quadrant, const sf::RenderWindow& window);

private:
    static constexpr float SPAWN_RADIUS = 200.0f;
};
```

#### `AnimatedBoardItem` (src/animated-board-item.h & .cpp)
Each diamond's individual saga of movement, a tale of flight and fury.
```cpp
// src/animated-board-item.h
#pragma once
#include <SFML/Graphics.hpp>

struct BoardAnimationConfig {
    float animationDuration = 2.5f;
    float rotationSpeed = 180.0f;
    float startScale = 0.3f;
    float endScale = 1.0f;
    float staggerDelay = 0.05f;
    bool enableRotation = true;
};

class AnimatedBoardItem {
public:
    void initialize(int id, sf::Vector2f spawn, sf::Vector2f target);
    void update(sf::Time deltaTime, const BoardAnimationConfig& config);
    void addToVertexArray(sf::VertexArray& vertices, int vertexOffset, const sf::Texture& texture) const;
    bool isFinished() const;

private:
    void initializeBezierPath(sf::Vector2f spawn, sf::Vector2f target);

    sf::Vector2f spawnPoint, targetPoint;
    sf::Vector2f bezierPoints[4];
    float progress = 0.0f;
    float rotationAngle = 0.0f;
    float currentScale = 0.3f;
    int diamondId = 0;
    bool finished = false;
};
```

#### `BoardInitializationAnimator` (src/board-initialization-animator.h & .cpp)
The great skald, who orchestrates the entire epic of the diamond animation.
```cpp
// src/board-initialization-animator.h
#pragma once
#include "animated-board-item.h"
#include "boarddiamondseq.h"
#include "board-spawn-regions.h"
#include <vector>
#include <SFML/Graphics.hpp>

class BoardInitializationAnimator {
public:
    void initializeAnimation(const BoardDiamondSeq& diamonds, sf::RenderWindow& window);
    void startAnimation();
    void update(sf::Time deltaTime);
    void render(sf::RenderTarget& target, const sf::Texture& texture) const;
    void skipAnimation();
    bool isComplete() const;

private:
    std::vector<AnimatedBoardItem> animatedItems;
    BoardSpawnRegions spawnRegions;
    BoardAnimationConfig config;
    sf::VertexArray animationVertices;
    bool animationComplete = true;
    float totalElapsedTime = 0.0f;
};
```

### Altering the Great Saga (State Management Integration)

#### `game.h` (src/game.h)
A new verse is added to the enum of states.
```cpp
// src/game.h
// ... existing code ...
enum states {
  state_init,
  state_menu,
  state_intro_shader,
  state_setup_players,
  state_board_animation,  // <-- A NEW VERSE IN OUR SAGA
  state_lets_begin,
  state_roll_dice,
// ... existing code ...
```

#### `GameStateManager` (src/game-state-manager.h & .cpp)
The keeper of sagas must learn the new transitions.
```cpp
// src/game-state-manager.h
// ...
void transitionToBoardAnimation();
void transitionFromBoardAnimationToLetsBegin();
// ...

// src/game-state-manager.cpp
// ...
void GameStateManager::transitionToBoardAnimation() {
    if (game->currentState != Game::state_board_animation) {
        handleStateChange(game->currentState, Game::state_board_animation);
        game->currentState = Game::state_board_animation;
        // The animator is awakened!
        game->boardAnimator->startAnimation();
    }
}

void GameStateManager::showGameBoard() {
  stopMenuMusic();
  startGameMusic();
  game->restartGame(); // This sets the final positions for the diamonds.

  // Instead of setting up players, we begin the great animation!
  transitionToBoardAnimation();
}
// ...
```

#### `GameRenderer` (src/game-renderer.h & .cpp)
The artist who paints our battles must learn to draw the animation.
```cpp
// src/game-renderer.h
// ...
void renderStateBoardAnimation();
// ...

// src/game-renderer.cpp
// In renderCurrentState() switch:
case Game::state_board_animation:
    renderStateBoardAnimation();
    break;

// ...
void GameRenderer::renderStateBoardAnimation() {
  drawBaseGame(); // The battlefield is drawn first.

  // The moving diamonds are rendered.
  game->boardAnimator->render(game->renderTexture, game->textures->textureBoardDiamond);

  // Other effects and characters join the fray.
  game->getAnimationSystem()->drawCircleParticles(game->renderTexture);
  drawCharacters();
}

```

#### `GameInput` (src/game-input.h & .cpp)
For the impatient warrior, a way to skip the grand display.
```cpp
// src/game-input.cpp
// In processKeyInput() switch:
case Game::state_board_animation:
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter) {
      game->boardAnimator->skipAnimation();
      game->stateManager->transitionFromBoardAnimationToLetsBegin();
    }
  }
  break;
```

## 4. The Raid Plan (Implementation Sequence)

### Phase 1: Forging the Tools (Core Animation Classes)
1.  **`BoardSpawnRegions`**: Carve the laws of quadrant mapping and player 2/3 correction into this class.
2.  **`AnimatedBoardItem`**: Teach this class the ways of the Bézier curve, of rotation and scaling.
3.  **`BoardInitializationAnimator`**: Bestow upon this class the leadership to command the `VertexArray` and the entire animation.

### Phase 2: Weaving the New Fate (State Integration)
1.  **Enrich `Game::states`**: Add `state_board_animation` to the sacred enum in `game.h`.
2.  **Educate `GameStateManager`**: Grant it the knowledge of the new transitions.
3.  **Instruct `GameRenderer`**: Teach it to render our new state with `renderStateBoardAnimation()`.
4.  **Enlighten `GameInput`**: Give it the power to skip the animation.

### Phase 3: The Spark of Life (Animation Trigger)
1.  **Modify `showGameBoard()`**: Change its destiny to trigger our animation state after `restartGame()`.
2.  **The Final Breath**: When the animation completes, it must automatically transition to `state_lets_begin`.
3.  **Preserving the Treasures**: The final positions of the diamonds must be identical to those calculated by `BoardDiamondSeq`. No diamond left behind!

### Phase 4: The Final Test (Fullscreen & Performance)
1.  **Universal Vision**: Use `mapPixelToCoords` for flawless fullscreen performance.
2.  **Mighty `VertexArray`**: A single draw call for all 112 diamonds, a true display of power!
3.  **A Light Footprint**: The animation must not tax the machine; it should use less than 5% of the CPU's might.

## 5. Runes of Power (Animation Parameters)

These values shall guide the spirit of the animation.
```cpp
struct BoardAnimationConfig {
    float animationDuration = 2.5f;     // How long the glorious charge lasts
    float rotationSpeed = 180.0f;       // Spinning like a berserker's axe
    float spawnRadius = 200.0f;         // Distance from the heart of the quadrant
    float startScale = 0.3f;            // A humble beginning
    float endScale = 1.0f;              // A mighty end
    float staggerDelay = 0.05f;         // A slight delay between each warrior's charge
    bool enableRotation = true;         // A toggle for the whirlwind effect
    // Easing is handled in AnimatedBoardItem::update
};
```

## 6. The Victory Feast (Expected Outcome)

From the four corners, the diamonds and runes will surge forth, a beautiful and chaotic charge, moving along smooth Bézier curves, rotating and scaling into their final, perfect positions. This grand spectacle will not only delight the eye but will be a testament to our coding might, maintaining perfect accuracy and performance. The skip function will grant control to the player, and our fullscreen implementation will ensure a flawless experience for all, no matter their window on the world. This conquest will integrate seamlessly into the existing game, a new, shining jewel in the crown of DeerPortal. 