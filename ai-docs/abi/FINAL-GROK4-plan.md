
# Final GROK4 Plan for Animated Board Initialization

## Overview

This plan synthesizes elements from ANIMATED_BOARD_INITIALIZATION.md, FINAL-plan.md, GROK4-plan.md, O3-plan.md, and corrections from CODEBASE-ANALYSIS.md and diamonds-grok4.md. It accounts for the current partial implementation in the codebase, including existing classes with TODO items, and incorporates the refactoring approach from GEMINI-FIX.md to ensure correct data flow and position accuracy.

The goal is to implement a smooth, quadrant-based animation for board diamonds (112 total) during game start or restart, without altering existing randomization logic. Animations include Bezier curve movement, rotation, scaling, and staggered timing, optimized with VertexArray batching and object pooling.

## Key Corrections and Considerations from Analysis

- **Quadrant Mapping Fix**: Player areas are indexed as: Player 0 (diamonds 0-27), Player 1 (28-55), Player 3 (56-83), Player 2 (84-111). Note the Player 2/3 swap in indexing.
- **Position Calculation**: Use DP::transPosition(boardPosition) to get grid coords, then DP::getScreenPos() + global offset (202, 76) + centering offset (2.4f, 2.4f).
- **Board Grid**: 16x16 grid (256 positions), divided into quadrants: Q0 (top-left: x<8,y<8), Q1 (top-right: x>=8,y<8), Q2 (bottom-left: x<8,y>=8), Q3 (bottom-right: x>=8,y>=8).
- **Current Implementation State**: Classes BoardInitializationAnimator, BoardSpawnRegions, AnimatedBoardItem exist but are incomplete (placeholders and TODOs). Integration in Game class is partial (boardAnimator initialized).
- **Data Flow Fix (from GEMINI-FIX.md)**: Prepare animation data in BoardDiamondSeq before starting animation to avoid using uninitialized positions.
- **Performance**: Leverage existing GameAnimationSystem for pooling and batching. Target <5% CPU, single VertexArray draw.
- **SFML 3.0 Compliance**: Use modern APIs, integrate with existing rendering pipeline.

## Architecture

### Components
1. **BoardSpawnRegions** (src/board-spawn-regions.h/cpp): Handles quadrant mapping and random off-screen spawn points. Complete getQuadrantForPosition() and getSpawnPoint() methods.
2. **AnimatedBoardItem** (src/animated-board-item.h/cpp): Manages individual item animation (Bezier path, rotation, scaling). Implement update() with easing, draw(), isFinished().
3. **BoardInitializationAnimator** (src/board-initialization-animator.h/cpp): Controller class. Extend to use vector of AnimatedBoardItem, integrate with GameAnimationSystem for pooling. Complete startAnimation(), update(), render() with VertexArray batching, initializeItems(), handleErrors().

### New Game State
- Add `state_board_animation` to enum in game.h, between `state_setup_players` and `state_lets_begin`.
- Update GameStateManager, GameRenderer, GameInput to handle the new state (add transition methods, renderStateBoardAnimation(), input handling for skip).

### Configuration
```cpp
struct BoardAnimationConfig {
  float animationDuration = 2.5f;
  float rotationSpeed = 180.0f;
  float spawnRadius = 200.0f;
  float startScale = 0.3f;
  float endScale = 1.0f;
  int maxConcurrentItems = 25;
  float staggerDelay = 0.05f;
  bool enableRotation = true;
  AnimationEasing easing = EASE_OUT_CUBIC;
};
```
Add to game-assets.h or similar, load from config if needed.

## Implementation Steps

### Phase 1: Complete Core Classes
1. In BoardSpawnRegions: Implement getSpawnPoint() to generate random points in off-screen annuli per quadrant.
2. In AnimatedBoardItem: Add Bezier[4] points, t progress, update() with cubic Bezier and easing, rotation/scale interpolation.
3. In BoardInitializationAnimator: 
   - In initializeItems(): Read prepared data from BoardDiamondSeq, create AnimatedBoardItem for each diamond, assign spawn points and targets.
   - Implement update(): Update active items, check completion.
   - Implement render(): Batch draw using sf::VertexArray.
   - Add error handling with try-catch and fallback to instant placement.

### Phase 2: Integrate with Game Flow
1. Modify Game::restartGame(): Call boardDiamonds.prepareAnimationData() to compute positions without rendering.
2. After restartGame() in GameStateManager::showGameBoard(), transition to state_board_animation and call boardAnimator->startAnimation().
3. In state_board_animation update: Call boardAnimator->update(dt); if isFinished(), transition to state_lets_begin and finalize BoardDiamondSeq vertices.
4. Add skip functionality: On key press, call boardAnimator->skipAnimation() and immediate transition.

### Phase 3: Optimization and Testing
1. Integrate object pooling via GameAnimationSystem.
2. Ensure VertexArray updates only when needed.
3. Add unit tests: Assert final positions match reorder() results, test quadrant mapping, seeded RNG for reproducibility.
4. Performance profiling: Verify <5% CPU during animation.
5. Fullscreen handling: Use window.mapPixelToCoords for positions.

## Integration Points
- Trigger: After restartGame() completes.
- Rendering: Temporary VertexArray during animation, switch to BoardDiamondSeq::m_vertices on completion.
- Input: Skip with key press (e.g., Space or Enter).
- Error Fallback: If animation fails, instantly place items using existing logic.

## Expected Outcome
Diamonds spawn from quadrant corners, animate smoothly to exact positions with visual effects, enhancing game start visuals while maintaining performance and position accuracy. 