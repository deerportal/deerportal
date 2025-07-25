# Final Optimized Board Animation Plan

## Synthesis Overview
This plan combines FINAL-plan.md's structure with GROK4-plan.md's optimizations, corrected per CODEBASE-ANALYSIS.md. Fixes: quadrant mapping (player 2/3 swap), state integration, error handling, fullscreen views, adaptive performance, testing, modularity.

## Core Architecture
- BoardInitializationAnimator: Main controller, extends GameAnimationSystem.
- BoardSpawnRegions: Quadrant mapping with corrected player 2/3 indexing.
- AnimatedBoardItem: Individual animation with Bezier paths.
- New state: state_board_animation.

## Position Corrections
- Quadrants: Player 0 (0-27), 1 (28-55), 3 (56-83), 2 (84-111) - note 2/3 swap.
- Positions: Use DP::transPosition and DP::getScreenPos + (202,76) offset + (2.4f centering).
- Fullscreen: Map via window.mapPixelToCoords for viewTiles.

## Animation Parameters
// As in FINAL-plan, with GROK4 waves
struct Config { ... };

## Implementation with Fixes
- Error Handling: Try-catch in start/update, fallback to instant placement.
- Performance: Adaptive stagger if frameTime > threshold, cap concurrent to 25.
- Testing: Assert position equality, seeded RNG.
- Modularity: Integrate with existing VertexArray. 