# Gemini Fix Plan: Diamond Animation and Placement Discrepancy

This document outlines the plan to fix the visual discrepancy between the diamond initialization animation and their final placement on the board.

## Problem

The pre-game animation for the diamonds does not align with their final rendered positions. This is due to a difference in the position calculation logic between `BoardInitializationAnimator` and `BoardDiamondSeq`.

- **`BoardDiamondSeq`**: Calculates the top-left position for each diamond sprite for static rendering.
- **`BoardInitializationAnimator`**: Calculates the center position of the diamond as the animation's target.

This leads to a visual "jump" when the animation completes and the board is rendered in its final state.

## Proposed Fix

The fix involves aligning the animation's target position with the final rendering position.

1.  **Modify `BoardInitializationAnimator::initializeAnimation`**:
    -   In `src/board-initialization-animator.cpp`, change the calculation of `targetPos`.
    -   It should use the top-left `staticPosition` directly, without adding `halfSize`. This will make the animation target the same point as the final rendering.

2.  **Verify `AnimatedBoardItem` Logic**:
    -   Review `src/animated-board-item.cpp` and `src/animated-board-item.h`.
    -   The `update` function in `AnimatedBoardItem` likely uses the position for translation and rotation. We need to ensure that changing the target position to be top-left instead of center doesn't negatively affect the visual representation of the animation (e.g. rotation). It's possible the `AnimatedBoardItem` expects a center point for its transformations.
    -   If `AnimatedBoardItem`'s logic relies on a center point, we must adjust it. The item's internal position should still represent its center for smooth rotation, but the movement interpolation should guide this center towards a final destination that *results* in the top-left corner being at `targetPos`.

## Implementation Steps

1.  Create a new `GEMINI-FIX.md` file with this plan.
2.  Apply the change to `src/board-initialization-animator.cpp`.
3.  Analyze and, if necessary, refactor `AnimatedBoardItem` to handle the position alignment correctly.
4.  Test the game to confirm that the animation is smooth and the diamonds land in their exact final positions without any visual stutter. 