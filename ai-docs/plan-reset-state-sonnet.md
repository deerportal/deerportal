# Game State Reset Fix Plan

## Problem Analysis

When players press Escape to return to the main menu and then start a new game, the game does NOT properly reset its state. The transition from menu back to game setup bypasses the `restartGame()` function, causing previous game state to persist inappropriately.

## Critical Issues Identified

### 1. Missing Reset Call in State Transition

**Location**: `src/game-input.cpp` lines 200-203  
**Issue**: `processMenuInput()` calls `showGameBoard()` without resetting game state  
**Current Code**:
```cpp
void GameInput::processMenuInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  game->downTimeCounter = 0;
  game->stateManager->hideMenu();
  game->stateManager->showGameBoard(); // Missing reset
}
```

### 2. Incomplete State Manager Implementation

**Location**: `src/game-state-manager.cpp` lines 103-110  
**Issue**: `showGameBoard()` sets state but doesn't reset game  
**Current Code**:
```cpp
void GameStateManager::showGameBoard() {
  stopMenuMusic();
  startGameMusic();
  game->currentState = Game::state_setup_players;
  game->sfx.playLetsBegin();
  // Missing: game->restartGame() call
}
```

## State Elements That Persist (Should Be Reset)

### Player State
- Player positions (`players[i].pos`)
- Character positions on board
- Player resources (cash, energy, food, faith)
- Player completion status (`players[i].done`)
- Portal reach status (`players[i].reachedPortal`)

### Game Progress
- Current player turn (`turn`)
- Round number (`roundNumber`)
- Season (`currentSeason`)
- Month (`month`)
- Number of finished players (`numberFinishedPlayers`)

### Board State
- Diamond positions
- Dice results (`diceResultPlayer`)
- Card deck state and order

### Visual/UI State
- Mouse selector position
- Player board element visibility (`showPlayerBoardElems`)
- UI element positioning from previous game state

## Fix Implementation Plan

### Option 1: Add Reset Call to showGameBoard()

**File**: `src/game-state-manager.cpp`  
**Modification**: Add `game->restartGame()` call in `showGameBoard()`

```cpp
void GameStateManager::showGameBoard() {
  stopMenuMusic();
  startGameMusic();
  game->restartGame(); // ADD THIS LINE
  game->currentState = Game::state_setup_players;
  game->sfx.playLetsBegin();
}
```

**Pros**: Simple, follows existing pattern  
**Cons**: Always resets when returning to game (may not be desired behavior)

### Option 2: Add Reset Call to processMenuInput()

**File**: `src/game-input.cpp`  
**Modification**: Add reset before state transition

```cpp
void GameInput::processMenuInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  game->downTimeCounter = 0;
  game->restartGame(); // ADD THIS LINE
  game->stateManager->hideMenu();
  game->stateManager->showGameBoard();
}
```

**Pros**: More explicit control in input handling  
**Cons**: Mixes game logic with input processing

### Option 3: Add Menu-Specific Reset Method

**Files**: `src/game-state-manager.h/cpp`  
**Modification**: Create `showNewGame()` method

```cpp
// In header
void showNewGame();

// In implementation
void GameStateManager::showNewGame() {
  stopMenuMusic();
  startGameMusic();
  game->restartGame();
  game->currentState = Game::state_setup_players;
  game->sfx.playLetsBegin();
}
```

**Pros**: Clear separation of concerns, allows for future "resume game" functionality  
**Cons**: Requires additional method and interface changes

## Recommended Solution

**Use Option 1** - Add `game->restartGame()` to `showGameBoard()` method.

**Rationale**:
- Simplest implementation
- Follows existing code patterns
- Ensures consistent behavior
- Players expect new game when returning from menu
- Minimal code changes required

## Additional Considerations

### AI/Player Configuration
- Currently AI vs Human settings persist (may be desirable)
- Consider if this should be preserved or reset
- May require storing these settings separately

### Future Enhancements
- Consider adding "Resume Game" vs "New Game" options to menu
- Implement proper save/load game state functionality
- Add confirmation dialog for abandoning current game

## Testing Requirements

After implementing fix, verify:
1. Escape from game → menu → click returns to fresh game state
2. Player positions reset to starting positions
3. All resources reset to initial values
4. Turn order and game progress reset
5. Card deck properly shuffled and reset
6. Visual elements properly initialized
7. Audio transitions work correctly

## Files to Modify

1. **Primary**: `src/game-state-manager.cpp` (add restartGame call)
2. **Verification**: Test state transitions and ensure proper reset
3. **Documentation**: Update relevant comments if needed

## Implementation Priority

**High Priority** - This is a user-facing bug that significantly impacts gameplay experience and player expectations of game state management.