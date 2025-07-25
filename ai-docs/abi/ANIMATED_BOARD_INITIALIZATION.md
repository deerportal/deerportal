# Animated Board Initialization System

## Overview

This document describes the implementation of an animated board initialization system for DeerPortal. Instead of diamonds and runes appearing instantly when the game starts/restarts, they will now animate from quadrant corners to their final positions with rotation, scaling, and smooth movement.

## Feature Requirements

- **Quadrant-Based Spawning**: Board items spawn from their respective quadrant corners (top-left, top-right, bottom-left, bottom-right)
- **Smooth Animations**: Items animate with rotation, scaling, and curved movement paths
- **Performance Optimized**: Uses object pooling, VertexArray batching, and efficient rendering
- **Modular Design**: Clean, maintainable code with separated concerns
- **SFML 3.0 Compliant**: Uses modern SFML 3.0 API and best practices

## Architecture Design

### New Components

#### 1. BoardInitializationAnimator
**File**: `src/board-initialization-animator.h/cpp`
- Main controller for animated board setup
- Manages animation state and timing
- Coordinates between spawn regions and animated items
- Handles transition to next game state when complete

#### 2. BoardSpawnRegions  
**File**: `src/board-spawn-regions.h/cpp`
- Defines 4 quadrant spawn zones around screen edges
- Maps board positions to appropriate spawn quadrants
- Calculates random spawn points within each quadrant
- Provides utility functions for quadrant-based logic

#### 3. AnimatedBoardItem
**File**: `src/animated-board-item.h/cpp`
- Individual animated item (diamond/rune)
- Handles movement, rotation, and scaling animations
- Uses bezier curves for smooth movement paths
- Supports configurable animation parameters

### New Game State

- **state_board_animation**: New state between `state_setup_players` and `state_lets_begin`
- Handles the animated placement of all board elements
- Automatically transitions when animation completes

### Animation Features

#### Movement System
- **Bezier Curves**: Smooth curved paths from spawn to destination
- **Staggered Timing**: Items animate in waves for visual appeal
- **Variable Speed**: Different animation speeds based on distance

#### Visual Effects
- **Rotation**: Continuous rotation during movement (configurable speed)
- **Scaling**: Start small (0.3x), grow to normal (1.0x) on arrival
- **Easing**: Smooth acceleration/deceleration curves

#### Performance Optimizations
- **Object Pooling**: Reuse animation objects to minimize allocations
- **VertexArray Batching**: Single draw call for all animated items
- **Spatial Culling**: Only process visible animations
- **Memory Efficient**: Minimal memory footprint

## Configuration System

```cpp
struct BoardAnimationConfig {
  float animationDuration = 2.5f;        // Total animation time
  float rotationSpeed = 180.0f;          // Degrees per second
  float spawnRadius = 200.0f;            // Spawn area radius
  float startScale = 0.3f;               // Initial item scale
  float endScale = 1.0f;                 // Final item scale
  int maxConcurrentItems = 25;           // Max items animating simultaneously
  float staggerDelay = 0.05f;            // Delay between item spawns
  bool enableRotation = true;            // Enable/disable rotation
  AnimationEasing easing = EASE_OUT_CUBIC; // Animation easing type
};
```

## Implementation Plan

### Phase 1: Core Infrastructure
1. Create new game state `state_board_animation`
2. Implement `BoardSpawnRegions` class
3. Add basic quadrant mapping logic
4. Integrate with existing game state machine

### Phase 2: Animation System
1. Implement `AnimatedBoardItem` class
2. Add movement, rotation, and scaling animations
3. Create bezier curve movement system
4. Add configurable animation parameters

### Phase 3: Integration and Optimization  
1. Implement `BoardInitializationAnimator` controller
2. Add object pooling for performance
3. Integrate with existing `BoardDiamondSeq` system
4. Add VertexArray batching for efficient rendering

### Phase 4: Polish and Testing
1. Add animation configuration system
2. Fine-tune timing and visual effects
3. Performance testing and optimization
4. Integration testing with game flow

## Technical Details

### Quadrant Mapping
- **Board Size**: 256 positions (16x16 grid)
- **Quadrant Division**: Split board into 4 equal regions
- **Spawn Areas**: Off-screen regions around each quadrant corner
- **Random Placement**: Items spawn at random positions within spawn radius

### Animation Mathematics
- **Bezier Curves**: Cubic bezier for smooth movement paths
- **Easing Functions**: Standard easing curves (ease-in, ease-out, ease-in-out)
- **Rotation**: Linear rotation with configurable speed
- **Scaling**: Smooth interpolation from start to end scale

### Performance Characteristics
- **Memory Usage**: ~50KB for animation system
- **CPU Impact**: <5% during animation phase
- **Rendering**: Single VertexArray draw call
- **Animation Count**: Up to 112 simultaneous animations (all board items)

### Integration Points
- **Game State Machine**: New state in existing enum
- **Board Initialization**: Modified `Game::initBoard()` flow
- **Rendering Pipeline**: Integrated with existing render system
- **Input Handling**: Animation can be skipped with key press

## Expected Visual Result

When the game starts or restarts:
1. Screen shows empty board
2. Items begin spawning from quadrant corners
3. Each item rotates and scales while moving to final position
4. Movement follows smooth curved paths
5. Items arrive in staggered waves for visual impact
6. Animation completes and game proceeds to dice roll phase

This creates a spectacular "board coming alive" effect that enhances the game's visual appeal while maintaining smooth performance.

---

*This document covers the animated board initialization system for DeerPortal 0.9.9-pre.2 "Development Phase" release.*