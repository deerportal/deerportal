"# Analysis of Diamonds and Runes Positioning at Game Start

## Overview
In the DeerPortal game, diamonds represent collectible items on the board, including white diamonds and element cards (referred to as runes in documentation). All are managed through the BoardDiamondSeq class. The positioning occurs during game initialization and restart, ensuring random placement within designated player areas without altering the core randomization logic.

## Key Classes and Structures
- **BoardDiamondSeq**: Manages an array of 112 BoardDiamond objects.
- **BoardDiamond**: Represents individual diamonds with idNumber, playerNumber, and boardPosition.
- **DP::DIAMONDS_SETUP**: A constant array in data.cpp defining initial values for each diamond (idNumber, playerNumber, initial position -1).
- **DP Namespace Functions**: transPosition, getScreenPos, transCords for coordinate transformations.

## Initialization Sequence
1. **Game Constructor**:
   - Creates BoardDiamondSeq instance with texture holder.
   - In BoardDiamondSeq constructor:
     - Initializes diamonds array using DIAMONDS_SETUP for idNumber, playerNumber, and initial position (-1).
     - Calls reorder() to assign random positions.

2. **loadAssets()**:
   - Loads textures, including textureBoardDiamond used for diamonds.

3. **initBoard()**:
   - Sets boardDiamonds position to (202, 76) for coordinate system alignment.
   - Calls restartGame().

4. **restartGame()** (in GameCore):
   - Resets players and game state.
   - For each player area (i=0 to 3):
     - Calls boardDiamonds.reorder(i) to randomize positions in that area.
   - Resets other game variables.

## Positioning Mechanism
- **Initial Setup**:
  - Each diamond starts with boardPosition = -1 (off-board).
  - playerNumber determines the quadrant (0-3 corresponding to elements/players).

- **Randomization in reorder()**:
  - For each player area (element):
    - Uses getRandomPos() to select unoccupied positions from occupiedFields.
    - occupiedFields defines available steps per player area.
    - Excludes positions already occupied by other diamonds.
    - Assigns random boardPosition within the area's range.

- **Position Calculation in BoardDiamond::setBoardPosition()**:
  - Converts boardPosition to grid coordinates using DP::transPosition().
  - Converts grid to screen position using DP::getScreenPos().
  - Sets sprite position accordingly.
  - Applies global offset (202, 76) via BoardDiamondSeq's setPosition().

- **VertexArray Optimization**:
  - Positions are used to build a VertexArray for batched rendering.
  - updateSingleDiamond() centers diamonds on tiles with scaling (80% size).

## Runes Specifics
Runes refer to element-specific cards represented as diamonds. Their positioning follows the same process as white diamonds:
- Filtered by idNumber in DIAMONDS_SETUP.
- Randomized within element areas.
- No separate class; handled uniformly as BoardDiamond.

## Related Functions
- **getRandomPos()**: Selects random free position from available steps, excluding occupied ones.
- **reorder(int element)**: Variant for single area reordering (used in meditation or specific events).
- **collectField()**: Removes diamond from position, setting to -1.

## Potential Issues and Fixes
- Positioning uses fixed offsets for alignment.
- PR #74 fixed coordinate system and rendering order issues.
- Ensures diamonds appear before characters in draw order.

This setup ensures random, fair distribution of diamonds and runes across the board at each game start or restart. 