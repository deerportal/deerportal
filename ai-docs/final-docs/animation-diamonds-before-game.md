# Diamond Animation System - Board Initialization

## Overview

The board initialization animation system creates a spectacular visual effect where all 112 diamonds explode radially from the center big diamond location to their final board positions in waves, accompanied by dynamic lighting effects.

## System Components

### 1. BoardInitializationAnimator
- **Purpose**: Manages the overall animation sequence
- **Key Features**:
  - Staggered diamond spawning (0.05s delay between diamonds)
  - Bezier curve movement paths for natural arcing motion
  - Lighting integration during animation
  - Hold state after animation completes

### 2. AnimatedBoardItem
- **Purpose**: Individual diamond animation logic
- **Features**:
  - Cubic Bezier path calculation
  - Eased scaling (0.1 to 1.0) - enhanced for center explosion
  - Rotation effects during movement
  - Progress tracking (0.0 to 1.0)

### 3. BoardSpawnRegions
- **Purpose**: Determines spawn points for each diamond
- **Logic**: All diamonds spawn from the big diamond center (521, 393) for radial explosion effect

## Animation States

1. **Initialization**: Diamonds positioned at center spawn point (big diamond center)
2. **Animation Phase**: Diamonds explode radially along gentle S-curve Bezier paths to targets
3. **Hold Phase**: Diamonds remain at final positions with lighting
4. **Release**: Animation system releases control to static board rendering

## Critical Fix: Diamond Disappearing Issue

### Problem
Diamonds were disappearing after completing their individual animations because the rendering logic only showed diamonds during their staggered start period.

### Root Cause
In `updateVertexArray()`, the condition was:
```cpp
if (totalElapsedTime >= staggeredStartTime) {
    // Render diamond
}
```

This meant once a diamond finished and new diamonds started, the finished ones became invisible.

### Solution
Modified the rendering condition to:
```cpp
bool hasStarted = totalElapsedTime >= staggeredStartTime;
bool isFinished = animatedItems[i].isFinished();

if (hasStarted || (isFinished && holdingDiamonds)) {
    // Render diamond
}
```

This ensures:
- Diamonds render during their animation phase
- Finished diamonds continue rendering when in hold state
- All diamonds remain visible until explicitly released

### Code Changes
1. **AnimatedBoardItem.h**: Added `getProgress()` method
2. **BoardInitializationAnimator.cpp**:
   - Modified `updateVertexArray()` rendering logic
   - Updated `updateLights()` to use progress-based condition
   - Fixed `render()` to continue rendering during hold state

## Lighting Integration

The animation system integrates with the LightingManager to create dynamic lighting effects:

- Each animated diamond generates a light source
- Light intensity scales with diamond size (0.3 to 1.0)
- Light radius: 60-100px based on scale
- Lights persist during hold state for seamless transition

## State Transitions

```
state_setup_players -> state_board_animation -> state_lets_begin -> state_roll_dice
```

1. User clicks "Start Game" in setup
2. Animation initializes and runs
3. Animation completes, enters hold state (state_lets_begin)
4. User clicks to proceed to gameplay (state_roll_dice)
5. Animated diamonds released, static diamonds take over

## Performance Optimizations

- VertexArray batching for efficient rendering
- Spatial culling for lighting (when 100+ lights active)
- Staggered updates to prevent frame drops
- Single texture atlas for all diamond types

## Configuration

```cpp
struct BoardAnimationConfig {
    float animationDuration = 2.5f;    // Total animation time
    float rotationSpeed = 180.0f;      // Degrees per second
    float spawnRadius = 200.0f;        // Distance from corners
    float startScale = 0.3f;           // Initial diamond size
    float endScale = 1.0f;             // Final diamond size
    float staggerDelay = 0.05f;        // Delay between diamonds
    bool enableRotation = true;        // Rotation during movement
};
```

## SFML 3.0.1 Compatibility

The system is fully compatible with SFML 3.0.1:
- Uses modern VertexArray rendering
- Proper texture coordinate mapping
- Compatible with new event system
- Efficient batch rendering techniques

## Debugging

Debug output can be enabled in non-release builds:
- Animation state transitions
- Individual diamond progress
- Lighting system status
- Vertex array updates

The system provides comprehensive logging for troubleshooting animation issues.