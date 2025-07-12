# DeerPortal C++ Coding Standards

**Document Version**: 1.0  
**Last Updated**: July 11, 2025  
**Style Base**: LLVM with DeerPortal-specific adaptations

## Overview

DeerPortal follows LLVM-based coding standards with project-specific customizations for SFML 3.0 game development. All C++ code is automatically formatted using clang-format to ensure consistency across the codebase.

## Code Formatting

### Automated Formatting

**Primary Tool**: clang-format with `.clang-format` configuration
```bash
# Format all files
clang-format -i src/*.cpp src/*.h

# Format specific file
clang-format -i src/game.cpp

# Check formatting without modifying
clang-format --dry-run --Werror src/*.cpp src/*.h
```

### Style Rules

#### Indentation and Spacing
```cpp
// ✅ CORRECT: 2-space indentation, no tabs
class GameRenderer {
public:
  void render() {
    if (isVisible) {
      drawElements();
    }
  }

private:
  bool isVisible;
};
```

#### Line Length and Breaking
```cpp
// ✅ CORRECT: 100-character line limit
void loadAssets(const std::string& basePath, 
               const std::vector<std::string>& assetPaths,
               bool enableCaching = true);

// ✅ CORRECT: Break long function calls
texture.loadFromFile(
    getAssetPath("textures/character_sprite.png"),
    sf::IntRect(0, 0, 64, 64));
```

#### Pointer and Reference Alignment
```cpp
// ✅ CORRECT: Left-aligned pointers and references
int* playerCount = nullptr;
const std::string& assetPath = getBasePath();
std::unique_ptr<sf::Texture> texture;

// ❌ INCORRECT: Right-aligned or mixed
int *playerCount = nullptr;     // Wrong
const std::string &assetPath;   // Wrong
```

#### Brace Style
```cpp
// ✅ CORRECT: Attach braces for control statements
if (condition) {
  doSomething();
}

while (gameRunning) {
  processEvents();
  update();
  render();
}

// ✅ CORRECT: Function braces on new line for classes
class GameCore {
public:
  void initialize() {
    // Implementation
  }
};
```

## Naming Conventions

### Variables and Functions
```cpp
// ✅ CORRECT: camelCase for variables and functions
int playerScore = 0;
bool isGameRunning = true;
void updateGameState();
sf::Vector2f getPlayerPosition();

// ❌ INCORRECT: Other conventions
int player_score;        // snake_case
bool IsGameRunning;      // PascalCase
void UpdateGameState();  // PascalCase
```

### Classes and Types
```cpp
// ✅ CORRECT: PascalCase for classes and types
class GameRenderer {
public:
  enum class RenderMode {
    Normal,
    Debug,
    Wireframe
  };
  
  struct RenderConfig {
    bool enableShaders;
    int maxParticles;
  };
};

// ✅ CORRECT: Type aliases
using TexturePtr = std::unique_ptr<sf::Texture>;
using PlayerList = std::vector<Player>;
```

### Constants and Enums
```cpp
// ✅ CORRECT: PascalCase for enum values
enum class Element {
  Fire,
  Water,
  Earth,
  Air
};

// ✅ CORRECT: ALL_CAPS for compile-time constants
constexpr int MAX_PLAYERS = 4;
constexpr float GRAVITY_CONSTANT = 9.81f;

// ✅ CORRECT: camelCase for const variables
const std::string assetBasePath = "/usr/share/deerportal/";
```

### File Names
```cpp
// ✅ CORRECT: Lowercase with hyphens for multi-word files
game-renderer.h
game-renderer.cpp
card-notification.h
error-handler.cpp

// ✅ CORRECT: Simple lowercase for single words
game.h
player.cpp
utils.h
```

## Include Organization

### Header Include Order
```cpp
// 1. Corresponding header (for .cpp files)
#include "game-renderer.h"

// 2. Standard C++ headers
#include <iostream>
#include <memory>
#include <vector>

// 3. SFML headers
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// 4. Project headers
#include "game-core.h"
#include "safe-asset-loader.h"
```

### Header Guards and Pragma
```cpp
// ✅ CORRECT: Traditional header guards
#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

// Header content...

#endif // GAME_RENDERER_H

// ✅ ALTERNATIVE: Pragma once (project preference)
#pragma once

// Header content...
```

## Modern C++17 Features

### Memory Management
```cpp
// ✅ CORRECT: Smart pointers for SFML objects
std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();

// ✅ CORRECT: RAII for resource management
class TextureManager {
private:
  std::vector<std::unique_ptr<sf::Texture>> textures;
public:
  // Automatic cleanup via destructors
};

// ❌ AVOID: Raw pointers for owned resources
sf::Texture* texture = new sf::Texture();  // Memory leak risk
```

### Auto and Type Deduction
```cpp
// ✅ CORRECT: Auto for complex types
auto playerPosition = sf::Vector2f(100.0f, 200.0f);
auto textureIter = textureMap.find("player");

// ✅ CORRECT: Explicit types for clarity
int playerCount = 4;           // Clear intent
bool gameRunning = true;       // Boolean clarity
float deltaTime = 0.016f;      // Precision matters

// ❌ AVOID: Auto when type isn't obvious
auto result = calculateSomething();  // What type is result?
```

### Range-Based Loops
```cpp
// ✅ CORRECT: Range-based loops
for (const auto& player : playerList) {
  player.update(deltaTime);
}

for (auto& [key, texture] : textureMap) {
  texture.loadFromFile(getAssetPath(key));
}

// ✅ CORRECT: Traditional loops when index needed
for (size_t i = 0; i < players.size(); ++i) {
  players[i].setId(i);
}
```

### Lambda Expressions
```cpp
// ✅ CORRECT: Lambda for callbacks
gameCore.setErrorCallback([](const std::string& error) {
  std::cerr << "Game Error: " << error << std::endl;
});

// ✅ CORRECT: Lambda for algorithms
std::sort(players.begin(), players.end(), 
         [](const Player& a, const Player& b) {
           return a.getScore() > b.getScore();
         });
```

## SFML-Specific Conventions

### Resource Loading
```cpp
// ✅ CORRECT: Safe asset loading with error handling
bool loadTexture(const std::string& filename) {
  if (!texture.openFromFile(getAssetPath(filename))) {
    logError("Failed to load texture: " + filename);
    return false;
  }
  return true;
}

// ✅ CORRECT: Use SFML 3.0 API
texture.openFromFile(path);   // Not loadFromFile()
font.openFromFile(path);      // Not loadFromFile()
```

### Event Handling
```cpp
// ✅ CORRECT: SFML 3.0 event handling
while (window.pollEvent(event)) {
  if (std::holds_alternative<sf::Event::Closed>(event)) {
    window.close();
  } else if (auto keyPressed = std::get_if<sf::Event::KeyPressed>(&event)) {
    handleKeyPress(keyPressed->code);
  }
}
```

### Rendering Best Practices
```cpp
// ✅ CORRECT: Efficient rendering with VertexArray
sf::VertexArray vertices(sf::PrimitiveType::Triangles, 6);
// Set up vertices...
window.draw(vertices);

// ✅ CORRECT: Batch drawing operations
window.clear();
// Draw all sprites...
window.display();
```

## Error Handling

### Exception Usage
```cpp
// ✅ CORRECT: Custom exceptions with context
class AssetLoadException : public std::runtime_error {
private:
  std::string filename;
public:
  AssetLoadException(const std::string& msg, const std::string& file)
    : std::runtime_error(msg), filename(file) {}
  
  const std::string& getFilename() const { return filename; }
};

// ✅ CORRECT: Exception handling in main
try {
  game.run();
} catch (const AssetLoadException& e) {
  std::cerr << "Asset Error: " << e.what() 
            << " (file: " << e.getFilename() << ")" << std::endl;
  return 1;
}
```

### Error Logging
```cpp
// ✅ CORRECT: Structured error logging
void logError(const std::string& message, const std::string& context = "") {
  std::cerr << "[ERROR] " << message;
  if (!context.empty()) {
    std::cerr << " (Context: " << context << ")";
  }
  std::cerr << std::endl;
}
```

## Documentation Standards

### Function Documentation
```cpp
/*!
 * \brief Renders the game world to the specified target
 * \param target The render target (window or texture)
 * \param deltaTime Time elapsed since last frame in seconds
 * \return true if rendering succeeded, false on error
 */
bool renderGame(sf::RenderTarget& target, float deltaTime);
```

### Class Documentation
```cpp
/*!
 * \class GameRenderer
 * \brief Handles all rendering operations for the game
 * 
 * The GameRenderer manages the graphics pipeline including:
 * - Sprite rendering with batching optimization
 * - Shader effects and post-processing
 * - UI overlay rendering
 * - Performance monitoring and FPS display
 */
class GameRenderer {
  // Class implementation...
};
```

## Code Organization

### File Structure
```
src/
├── game.h/.cpp              # Main game class
├── game-core.h/.cpp         # Core game logic
├── game-renderer.h/.cpp     # Rendering system
├── game-input.h/.cpp        # Input handling
├── game-assets.h/.cpp       # Asset management
├── error-handler.h/.cpp     # Error handling
└── safe-asset-loader.h/.cpp # Asset loading utilities
```

### Namespace Usage
```cpp
// ✅ CORRECT: Project namespace
namespace DP {
  class Game {
    // Implementation...
  };
  
  namespace Utils {
    std::string getAssetPath(const std::string& relativePath);
  }
}

// ✅ CORRECT: Usage
DP::Game game;
std::string path = DP::Utils::getAssetPath("textures/player.png");
```

## Build Integration

### CMake Integration
```cmake
# Enable clang-format checking in CI
find_program(CLANG_FORMAT_EXECUTABLE clang-format)
if(CLANG_FORMAT_EXECUTABLE)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXECUTABLE} -i src/*.cpp src/*.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting C++ source code"
  )
endif()
```

### Pre-commit Hooks
```bash
#!/bin/bash
# .git/hooks/pre-commit
echo "Running clang-format..."
clang-format -i src/*.cpp src/*.h

if ! git diff --quiet; then
  echo "Code was reformatted. Please review and commit again."
  exit 1
fi
```

## Quality Metrics

### Code Review Checklist
- [ ] All files formatted with clang-format
- [ ] Naming conventions followed
- [ ] Include order correct
- [ ] No raw pointers for owned resources
- [ ] Error handling implemented
- [ ] Documentation added for public APIs
- [ ] SFML 3.0 API used correctly

### Automated Checks
```bash
# Format check (CI)
clang-format --dry-run --Werror src/*.cpp src/*.h

# Static analysis
clang-tidy src/*.cpp -- -std=c++17

# Build verification
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

---

This document establishes the coding standards for professional C++ development in the DeerPortal project, ensuring maintainability, readability, and consistency across all contributions.