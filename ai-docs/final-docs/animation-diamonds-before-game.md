# Board Diamond Animation Documentation

**Source Commit**: b38430fdfbe036993a94a6b37c68a34cfe6e1d22  
**Last Updated**: 2025-07-30

## Overview

DeerPortal implements a spectacular board initialization sequence where all 112 diamond pieces explode radially from the center of the screen to their final board positions. This animation is accompanied by dynamic lighting effects and a board-unveiling fade-out, creating a polished and engaging start to the game.

## Core Components

### BoardInitializationAnimator (`src/board-initialization-animator.h`)
The main class that orchestrates the diamond animation sequence.

#### Key Properties
- `std::vector<AnimatedBoardItem> animatedItems`: Collection of animated diamonds.
- `BoardSpawnRegions spawnRegions`: Defines that all diamonds spawn from the center.
- `BoardAnimationConfig config`: Animation parameters.
- `sf::VertexArray animationVertices`: Optimized rendering array.
- `bool animationComplete`, `bool holdingDiamonds`, `bool fadingOut`: Flags that manage the animation lifecycle.

### AnimatedBoardItem (`src/animated-board-item.h`)
Individual diamond piece animation controller.

#### Animation Properties
- `spawnPoint`, `targetPoint`: Start and end positions for the animation.
- `bezierPoints[4]`: Control points for a smooth, curved S-path.
- `progress`: Animation completion (0.0 to 1.0).
- `rotationAngle`, `currentScale`: Manages the diamond's rotation and size during animation.

## Animation Lifecycle

The animation proceeds through several distinct phases, managed by the `BoardInitializationAnimator`:

1.  **Animation Phase**: All 112 diamonds are created at the center of the screen and animate outwards to their final positions along gentle S-curve Bezier paths. Their scale and rotation are interpolated over the animation duration.
2.  **Hold Phase**: Once all diamonds have reached their destinations, they are held in place, and their lighting effects remain active.
3.  **Fade-Out Phase**: Immediately after the hold phase begins, a 2-second fade-out of the dark ambient overlay starts. The `getCurrentAmbientColor()` method in the animator interpolates the color from dark to bright.
4.  **Completion**: The animator's `isComplete()` method returns `true` only after the fade-out is finished. This triggers an automatic transition directly to `state_roll_dice`, bypassing the problematic `state_lets_begin` that caused dark background flashes.

## Visual Effects

- **Radial Explosion**: All diamonds spawning from a single point creates a dramatic "explosion" effect.
- **Scaling**: Diamonds start small (10% of final size) and grow as they travel, adding to the sense of motion.
- **Lighting**: Each diamond emits a light that illuminates the dark board, and the entire scene brightens as the dark overlay fades away.
