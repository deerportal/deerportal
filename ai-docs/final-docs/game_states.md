# Game States Documentation

**Source Knowledge**: Commit 60a939ef8d8de62a00d82cdf3da244d8a17463ea

## Overview

DeerPortal implements a state machine through the `GameStateManager` class (`src/game-state-manager.h/cpp`) that manages transitions between different game phases and handles associated audio/visual effects.

## State Definitions

The game defines the following states in `src/game.h`:

### Core States
- `state_init`: Initial application startup
- `state_menu`: Main menu with player setup options
- `state_intro_shader`: Animated intro sequence with shader effects
- `state_setup_players`: Player configuration screen
- `state_board_animation`: Animated board initialization sequence
- `state_lets_begin`: Pre-game countdown display
- `state_roll_dice`: Dice rolling phase
- `state_game`: Active gameplay state
- `state_gui_elem`: GUI element interaction
- `state_select_building`: Building selection mode
- `state_gui_end_round`: End of round interface
- `state_end_game`: Game completion screen
- `state_quit`: Application termination

## State Transitions

### Primary Transition Methods

#### `transitionToMenu()`
- Validates current state is not already menu
- Calls `handleStateChange()` for audio/visual effects
- Executes `showMenu()` to display menu interface

#### `transitionToGame()`
- Direct transition to active gameplay
- Sets `currentState = Game::state_game`

#### `transitionToSetup()`
- Transitions to player setup screen
- Calls `showGameBoard()` to prepare board display

#### `transitionToIntroShader()`
- Activates intro shader animation sequence
- Calls `showIntroShader()` for visual effects

## State Transitions

### Primary Transition Methods

#### `transitionToBoardAnimation()`
- Activates animated board initialization.
- Calls `game->boardAnimator->startAnimation()`.

#### `transitionFromBoardAnimationToLetsBegin()` - **DEPRECATED**
- This transition method is **no longer used** as of the fadeout fix.
- **Issue**: This method caused unwanted dark background rendering via `state_lets_begin`.
- **New Behavior**: Direct transition from `state_board_animation` to `state_roll_dice` bypasses this intermediate state.

#### **NEW: Direct Board Animation to Dice Transition**
- **Location**: `Game::update()` in `state_board_animation` case
- **Trigger**: When `boardAnimator->isComplete()` returns `true` (after fade-out completes)
- **Implementation**: Direct state assignment + `launchNextPlayer()` call
- **Result**: Seamless transition without dark background flash


#### `transitionToEndGame()`
- Handles game completion
- Calls `endGame()` for cleanup

#### `transitionToRollDice()`
- Activates dice rolling phase
- Direct state assignment

## State Management Features

### State Queries
- `getCurrentState()`: Returns current state integer
- `getCurrentStateName()`: Returns human-readable state name
- `isInGameState()`/`isInMenuState()`: Boolean state checks
- `canTransitionTo()`: Validates transition legality

### State Validation
- `validateStateTransition()`: Checks if transition is allowed
- `isValidState()`: Validates state integer values
- `logStateTransition()`: Debug logging for state changes

### Audio Integration
- `playStateMusic()`: Starts state-specific music
- `stopStateMusic()`: Stops current state music
- `handleStateAudio()`: Manages audio transitions

## Audio State Management

The system manages different music tracks for states:
- Menu states use menu music tracks
- Game states use gameplay music
- Transition effects include audio coordination

### Audio Methods
- `startMenuMusic()`/`stopMenuMusic()`: Menu audio control
- `startGameMusic()`/`stopGameMusic()`: Game audio control
- State-specific audio handling in `handleStateAudio()`

## Implementation Details

### State Change Handler
The `handleStateChange(int from, int to)` method:
- Manages audio transitions between states
- Handles visual effect coordination
- Ensures proper cleanup of previous state
- Prepares resources for new state

### State Preparation Methods
- `prepareMenuState()`: Menu initialization
- `prepareGameState()`: Game setup
- `prepareSetupState()`: Player setup preparation
- `prepareEndGameState()`: End game cleanup

## Integration with Game Flow

The state manager coordinates with:
- `BoardInitializationAnimator` for board animation state
- Audio system for state-specific music
- Main game loop for state-dependent rendering
- Input system for state-appropriate event handling

The state machine ensures proper game flow progression and prevents invalid state transitions while managing associated resources and effects.