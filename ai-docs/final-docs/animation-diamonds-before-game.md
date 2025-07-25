# Board Diamond Animation Documentation

**Source Knowledge**: Commit 60a939ef8d8de62a00d82cdf3da244d8a17463ea

## Overview

DeerPortal implements an animated board initialization sequence where diamond pieces appear on the game board before gameplay begins. This is handled by the `BoardInitializationAnimator` class and related components.

## Core Components

### BoardInitializationAnimator (`src/board-initialization-animator.h`)
The main class that orchestrates the diamond animation sequence.

#### Key Properties
- `std::vector<AnimatedBoardItem> animatedItems`: Collection of animated diamonds
- `BoardSpawnRegions spawnRegions`: Defines where diamonds spawn from
- `BoardAnimationConfig config`: Animation parameters
- `sf::VertexArray animationVertices`: Optimized rendering array
- `bool animationComplete`: Tracks completion state
- `float totalElapsedTime`: Animation timing tracker

### AnimatedBoardItem (`src/animated-board-item.h`)
Individual diamond piece animation controller.

#### Animation Properties
- `spawnPoint`: Starting position for diamond
- `targetPoint`: Final board position
- `bezierPoints[4]`: Bezier curve control points for smooth movement
- `progress`: Animation completion (0.0 to 1.0)
- `rotationAngle`: Current rotation state
- `currentScale`: Size scaling during animation
- `diamondId`: Unique identifier for diamond
- `textureId`: Atlas texture coordinate selector

## Animation Configuration

### BoardAnimationConfig Structure
```cpp
struct BoardAnimationConfig {
  float animationDuration = 2.5f;    // Total animation time
  float rotationSpeed = 180.0f;      // Degrees per second rotation
  float spawnRadius = 200.0f;        // Distance from center for spawn points
  float startScale = 0.3f;           // Initial diamond size
  float endScale = 1.0f;             // Final diamond size
  float staggerDelay = 0.05f;        // Delay between diamond animations
  bool enableRotation = true;        // Enable rotation during movement
};
```

## Animation Sequence

### Initialization Phase
1. `initializeAnimation()` called with `BoardDiamondSeq` and window reference
2. Spawn regions calculated around board center
3. Each diamond gets assigned spawn point and target position
4. Vertex array initialized for efficient rendering

### Animation Phase
1. `startAnimation()` begins the sequence
2. `update(sf::Time deltaTime)` called each frame:
   - Updates each `AnimatedBoardItem` progress
   - Applies easing functions for smooth movement
   - Updates rotation and scaling
   - Rebuilds vertex array for rendering

### Completion Phase
1. Individual diamonds complete when `progress >= 1.0f`
2. Overall animation completes when all diamonds finish
3. `isComplete()` returns true when sequence finished
4. Game state transitions to next phase

## Movement Mechanics

### Bezier Curve Trajectories
Each diamond follows a cubic Bezier curve from spawn to target:
- `initializeBezierPath()` calculates control points
- `calculateBezierPosition(float t)` computes position at time t
- Provides smooth, curved movement rather than linear paths

### Easing Functions
- `easeOutCubic(float t)`: Smooth deceleration at end of movement
- Creates natural-feeling animation curves
- Applied to position, scale, and rotation changes

## Visual Effects

### Scaling Animation
- Diamonds start at 30% size (`startScale = 0.3f`)
- Gradually scale to full size (`endScale = 1.0f`)
- Creates "growing" appearance effect

### Rotation Effects
- Continuous rotation during movement (`rotationSpeed = 180.0f`)
- Can be disabled via `enableRotation = false`
- Adds dynamic visual interest

### Staggered Timing
- `staggerDelay = 0.05f` creates wave-like appearance
- Prevents all diamonds from moving simultaneously
- Creates more visually appealing sequence

## Position Calculation

Position calculations use the exact same logic as BoardDiamondSeq for pixel-perfect alignment:

```cpp
// Use EXACT same position calculation as BoardDiamondSeq
const BoardDiamond& diamond = diamonds.diamonds[i];
sf::Vector2i cords = DP::transPosition(diamond.boardPosition);
sf::Vector2f tilePos = DP::getScreenPos(cords);

// IDENTICAL centering offset as BoardDiamondSeq::updateSingleDiamond()
const float offsetX = 2.4f;      // (40 - 35.2) / 2 = 2.4f
const float offsetY = 2.4f;      // (40 - 35.2) / 2 = 2.4f
sf::Vector2f staticPosition(tilePos.x + offsetX, tilePos.y + offsetY);

// Convert to center position for animation (diamondSize = 35.2f)
const float halfSize = 35.2f * 0.5f; // 17.6f
sf::Vector2f targetPos(staticPosition.x + halfSize, staticPosition.y + halfSize);
```

This ensures seamless transition from animated to static diamonds.

## Vertex Array Rendering

- Uses sf::PrimitiveType::Triangles with 6 vertices per diamond (2 triangles forming a quad)
- Supports rotation and scaling per diamond
- Single draw call for all 112 diamonds
- Dirty flag optimization prevents unnecessary rebuilds

## Visual Effects

### Scaling Animation
- Diamonds start at 30% size (`startScale = 0.3f`)
- Gradually scale to full size (`endScale = 1.0f`)
- Creates "growing" appearance effect

### Rotation Effects
- Continuous rotation during movement (`rotationSpeed = 180.0f`)
- Can be disabled via `enableRotation = false`
- Adds dynamic visual interest

### Staggered Timing
- `staggerDelay = 0.05f` creates wave-like appearance
- Prevents all diamonds from moving simultaneously
- Creates more visually appealing sequence

## Rendering Optimization

### Vertex Array Implementation
- All diamonds rendered in single draw call
- `updateVertexArray()` rebuilds geometry each frame
- Efficient batch rendering for multiple sprites

### Texture Atlas Integration
- `textureId` selects appropriate sprite from atlas
- Supports different diamond types/colors
- Optimized memory usage through texture sharing

## Game State Integration

### State Transitions
1. Game enters `state_board_animation`
2. `BoardInitializationAnimator::startAnimation()` called
3. Animation runs until completion
4. `transitionFromBoardAnimationToLetsBegin()` advances game state
5. Normal gameplay begins

### Skip Functionality
- `skipAnimation()` method for immediate completion
- Allows players to bypass animation if desired
- Maintains game flow flexibility

## Performance Considerations

### Frame Rate Optimization
- Vertex array prevents per-diamond draw calls
- Bezier calculations cached for smooth performance
- Delta time-based animation for frame rate independence

### Memory Management
- Efficient vector storage for animated items
- Reuse of vertex array across frames
- Cleanup of completed animations

The board diamond animation system provides an engaging visual introduction to each game while maintaining high performance through optimized rendering techniques.