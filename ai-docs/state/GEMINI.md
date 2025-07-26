# State Management Refactoring Plan (Revised)

This document outlines a revised plan to refactor the game's state management system to improve its efficiency, simplicity, and robustness.

## 1. Current State Summary

The game currently uses a `states` enum defined in `src/game.h` to manage the game's state. The `GameStateManager` class is responsible for handling state transitions.

The current states are:
- `state_init`
- `state_menu`
- `state_intro_shader`
- `state_setup_players`
- `state_board_animation`
- `state_lets_begin`
- `state_roll_dice`
- `state_game`
- `state_gui_elem`
- `state_select_building`
- `state_gui_end_round`
- `state_end_game`
- `state_quit`

While this system is functional, it can be improved by adopting a more object-oriented approach. The current implementation relies on a large `switch` statement in `GameStateManager::getCurrentStateName()` and direct manipulation of the `currentState` variable in the `Game` class, which can make the code harder to maintain and extend.

## 2. Convert `states` enum to a scoped enum (`enum class`)

To improve type safety and prevent naming conflicts, the `states` enum in `src/game.h` will be converted to a scoped enum.

**File to modify:** `src/game.h`

```cpp
// Game state enum moved to public for module access
enum class GameState {
    Init,
    Menu,
    IntroShader,
    SetupPlayers,
    BoardAnimation,
    LetsBegin,
    RollDice,
    Game,
    GuiElem,
    SelectBuilding,
    GuiEndRound,
    EndGame,
    Quit
};
GameState currentState;
```

## 3. Implement a State-Based Design Pattern

We will refactor the current state management to use a state-based design pattern. This will involve creating a `BaseState` class and a concrete state class for each game state.

### 3.1. Create a `BaseState` Class

A `BaseState` class will define the common interface for all game states.

**File to create:** `src/base-state.h`

```cpp
#pragma once

#include "game.h"

namespace DP {
    class BaseState {
    public:
        virtual ~BaseState() = default;
        virtual void handleInput(const sf::Event& event) = 0;
        virtual void update(sf::Time dt) = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
    };
}
```

### 3.2. Create Concrete State Classes

Concrete state classes will be created for each game state. Each class will inherit from `BaseState` and implement the state-specific logic. For example, a `MenuState` would be:

**File to create:** `src/menu-state.h`
```cpp
#pragma once

#include "base-state.h"

namespace DP {
    class MenuState : public BaseState {
    public:
        MenuState(Game& game);
        void handleInput(const sf::Event& event) override;
        void update(sf::Time dt) override;
        void draw(sf::RenderWindow& window) override;
    private:
        Game& game;
    };
}
```

## 4. Refactor `GameStateManager`

The `GameStateManager` will be refactored to manage a stack of `BaseState` pointers. This will allow for more flexible state transitions, such as pushing and popping states (e.g., for a pause menu).

**File to modify:** `src/game-state-manager.h`

```cpp
#pragma once

#include <memory>
#include <stack>
#include "base-state.h"
#include "game.h"

namespace DP {
    class GameStateManager {
    public:
        GameStateManager(Game& game);
        void pushState(GameState state);
        void popState();
        void changeState(GameState state);
        void handleInput(const sf::Event& event);
        void update(sf::Time dt);
        void draw(sf::RenderWindow& window);
    private:
        Game& game;
        std::stack<std::unique_ptr<BaseState>> states;
    };
}
```

## 5. Update the `Game` Class and Game Loop

The `Game` class will be updated to use the new `GameStateManager`. The main game loop in `game.cpp` will be simplified to delegate input, update, and draw calls to the `GameStateManager`.

**File to modify:** `src/game.cpp`

```cpp
// In Game::run()
while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        stateManager->handleInput(event);
    }

    stateManager->update(clock.restart());
    window.clear();
    stateManager->draw(window);
    window.display();
}
```

This revised plan provides a clear path to a more robust, scalable, and maintainable state management system.