# DeerPortal Error Handling Improvement Plan

**Date**: 2025-06-20  
**Priority**: CRITICAL  
**Impact**: Application stability, resource management, debugging capability

## Executive Summary

The DeerPortal codebase currently uses `std::exit()` for error handling in 7+ files with 30+ occurrences. This anti-pattern prevents proper cleanup, makes testing difficult, and can lead to resource leaks. This plan outlines a systematic approach to implement proper exception-based error handling throughout the codebase.

## Current State Analysis

### Files Affected by std::exit() Usage
- `game.cpp` - 12 occurrences (texture/sound loading failures)
- `game-assets.cpp` - 9 occurrences (font/asset loading failures)  
- `bubble.cpp` - 2 occurrences (texture loading failures)
- `textureholder.cpp` - Multiple occurrences
- `rounddice.cpp` - Texture loading failures
- `rotateelem.cpp` - Asset loading failures
- `guiwindow.cpp` - UI resource failures

### Problem Categories
1. **Asset Loading Failures** (90% of cases)
   - Missing texture files
   - Missing font files
   - Missing sound files
   - Invalid file formats

2. **System Resource Failures** (10% of cases)
   - Window creation failures
   - OpenGL context failures
   - Memory allocation failures

## Proposed Exception Hierarchy

```cpp
// File: src/exceptions.h
#pragma once
#include <exception>
#include <string>
#include <string_view>

namespace DeerPortal {

// Base exception class for all DeerPortal errors
class GameException : public std::exception {
protected:
    std::string message;
    std::string context;
    
public:
    GameException(std::string_view msg, std::string_view ctx = "") 
        : message(msg), context(ctx) {}
    
    const char* what() const noexcept override { 
        return message.c_str(); 
    }
    
    const std::string& getContext() const { return context; }
};

// Asset-related exceptions
class AssetLoadException : public GameException {
public:
    enum AssetType { TEXTURE, FONT, SOUND, SHADER };
    
private:
    std::string filename;
    AssetType type;
    
public:
    AssetLoadException(AssetType t, std::string_view file, std::string_view msg)
        : GameException(msg, file), filename(file), type(t) {}
        
    const std::string& getFilename() const { return filename; }
    AssetType getAssetType() const { return type; }
};

// System resource exceptions
class SystemResourceException : public GameException {
public:
    enum ResourceType { WINDOW, OPENGL_CONTEXT, MEMORY };
    
private:
    ResourceType type;
    
public:
    SystemResourceException(ResourceType t, std::string_view msg)
        : GameException(msg), type(t) {}
        
    ResourceType getResourceType() const { return type; }
};

// Configuration exceptions
class ConfigurationException : public GameException {
public:
    ConfigurationException(std::string_view msg) : GameException(msg) {}
};

// Game state exceptions
class GameStateException : public GameException {
public:
    GameStateException(std::string_view msg) : GameException(msg) {}
};

} // namespace DeerPortal
```

## Implementation Plan

### Phase 1: Foundation (Week 1)

#### 1.1 Create Exception Infrastructure
```cpp
// File: src/error-handler.h
#pragma once
#include "exceptions.h"
#include <functional>
#include <optional>

namespace DeerPortal {

class ErrorHandler {
public:
    using ErrorCallback = std::function<void(const GameException&)>;
    
    // Global error handler instance
    static ErrorHandler& getInstance();
    
    // Set custom error callback (for UI notifications)
    void setErrorCallback(ErrorCallback callback);
    
    // Handle exceptions with recovery options
    bool handleException(const GameException& e);
    
    // Try to load asset with fallback
    template<typename T>
    std::optional<T> tryLoadWithFallback(
        std::function<T()> loader,
        std::function<T()> fallbackLoader
    );
    
    // Log error to file
    void logError(const GameException& e);
    
private:
    ErrorCallback errorCallback;
    std::ofstream errorLog;
};

} // namespace DeerPortal
```

#### 1.2 Asset Loading Wrapper
```cpp
// File: src/safe-asset-loader.h
#pragma once
#include "exceptions.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace DeerPortal {

class SafeAssetLoader {
public:
    // Load texture with proper error handling
    static void loadTexture(sf::Texture& texture, const std::string& filename);
    
    // Load font with fallback support
    static void loadFont(sf::Font& font, const std::string& filename, 
                         const std::string& fallbackFont = "");
    
    // Load sound with error recovery
    static void loadSound(sf::SoundBuffer& buffer, const std::string& filename);
    
    // Try multiple paths for asset loading
    static std::string findAssetPath(const std::string& filename);
    
private:
    static std::vector<std::string> searchPaths;
};

} // namespace DeerPortal
```

### Phase 2: Systematic Replacement (Week 2)

#### 2.1 Replace std::exit() in game-assets.cpp
```cpp
// BEFORE:
if (!gameFont.openFromFile(get_full_path("assets/fonts/font.ttf"))) {
    std::cerr << "Failed to load font!" << std::endl;
    std::exit(1);
}

// AFTER:
try {
    SafeAssetLoader::loadFont(gameFont, "assets/fonts/font.ttf");
} catch (const AssetLoadException& e) {
    // Try fallback font
    try {
        SafeAssetLoader::loadFont(gameFont, "assets/fonts/fallback.ttf");
        ErrorHandler::getInstance().logError(e);
    } catch (const AssetLoadException& fallbackError) {
        // Critical error - cannot continue without any font
        throw AssetLoadException(
            AssetLoadException::FONT,
            e.getFilename(),
            "Failed to load both primary and fallback fonts"
        );
    }
}
```

#### 2.2 Update main() Function
```cpp
// File: src/main.cpp
int main(int argc, char** argv) {
    try {
        // Parse command line arguments
        bool testMode = (argc > 1 && std::string(argv[1]) == "--test");
        
        // Initialize error handler
        DeerPortal::ErrorHandler::getInstance().setErrorCallback(
            [](const DeerPortal::GameException& e) {
                std::cerr << "Game Error: " << e.what() << std::endl;
                if (!e.getContext().empty()) {
                    std::cerr << "Context: " << e.getContext() << std::endl;
                }
            }
        );
        
        // Create and run game
        DeerPortal::Game game(testMode);
        return game.run(); // Note: run() is the new method, not constructor
        
    } catch (const DeerPortal::AssetLoadException& e) {
        std::cerr << "Critical Asset Error: " << e.what() << std::endl;
        std::cerr << "Failed to load: " << e.getFilename() << std::endl;
        return 1;
    } catch (const DeerPortal::SystemResourceException& e) {
        std::cerr << "System Resource Error: " << e.what() << std::endl;
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
        return 3;
    }
}
```

### Phase 3: Enhanced Error Recovery (Week 3)

#### 3.1 Graceful Degradation Support
```cpp
// File: src/game-assets.cpp
void GameAssets::loadAllAssets(Game* game) {
    AssetLoadingReport report;
    
    // Load critical assets (game cannot run without these)
    try {
        loadCriticalAssets(game);
    } catch (const AssetLoadException& e) {
        throw; // Re-throw critical errors
    }
    
    // Load optional assets (game can run with defaults)
    loadOptionalAssetsWithFallback(game, report);
    
    // Show warning if some assets failed
    if (report.hasFailures()) {
        game->showAssetWarning(report);
    }
}

void GameAssets::loadOptionalAssetsWithFallback(Game* game, AssetLoadingReport& report) {
    // Example: Load character textures with colored rectangle fallback
    for (const auto& charTexture : characterTextures) {
        try {
            SafeAssetLoader::loadTexture(game->textures[charTexture.id], charTexture.path);
        } catch (const AssetLoadException& e) {
            // Create colored rectangle as fallback
            sf::Image fallback;
            fallback.create(sf::Vector2u(64, 64), charTexture.fallbackColor);
            game->textures[charTexture.id].loadFromImage(fallback);
            report.addFailure(charTexture.path, "Using colored rectangle fallback");
        }
    }
}
```

#### 3.2 Error Recovery UI
```cpp
// File: src/error-dialog.h
class ErrorDialog {
public:
    enum Action { RETRY, IGNORE, QUIT };
    
    static Action show(sf::RenderWindow& window, const GameException& error) {
        // Display error dialog with options
        // Returns user's choice
    }
    
    static void showAssetReport(sf::RenderWindow& window, 
                                const AssetLoadingReport& report) {
        // Show which assets failed to load
        // Allow user to continue or quit
    }
};
```

### Phase 4: Testing Infrastructure (Week 4)

#### 4.1 Unit Tests for Error Handling
```cpp
// File: tests/error-handling-test.cpp
#include <gtest/gtest.h>
#include "exceptions.h"
#include "safe-asset-loader.h"

TEST(ErrorHandling, AssetLoadExceptionConstruction) {
    AssetLoadException e(AssetLoadException::TEXTURE, "test.png", "File not found");
    EXPECT_EQ(e.getAssetType(), AssetLoadException::TEXTURE);
    EXPECT_EQ(e.getFilename(), "test.png");
    EXPECT_STREQ(e.what(), "File not found");
}

TEST(ErrorHandling, SafeAssetLoaderMissingFile) {
    sf::Texture texture;
    EXPECT_THROW(
        SafeAssetLoader::loadTexture(texture, "nonexistent.png"),
        AssetLoadException
    );
}

TEST(ErrorHandling, SafeAssetLoaderWithFallback) {
    sf::Font font;
    // Should succeed with fallback
    EXPECT_NO_THROW(
        SafeAssetLoader::loadFont(font, "missing.ttf", "assets/fonts/fallback.ttf")
    );
}
```

#### 4.2 Integration Tests
```cpp
TEST(Integration, GameStartupWithMissingAssets) {
    // Rename some assets to simulate missing files
    TestHelper::hideAsset("assets/fonts/font.ttf");
    
    // Game should still start with fallbacks
    Game game(true);
    EXPECT_NO_THROW(game.initialize());
    
    // Restore assets
    TestHelper::restoreAsset("assets/fonts/font.ttf");
}
```

## Migration Checklist

### Week 1 Tasks
- [ ] Create `src/exceptions.h` with exception hierarchy
- [ ] Create `src/error-handler.h/cpp` with error handling utilities
- [ ] Create `src/safe-asset-loader.h/cpp` with wrapped asset loading
- [ ] Update CMakeLists.txt to include new files
- [ ] Create unit tests for exception classes

### Week 2 Tasks
- [ ] Replace std::exit() in game-assets.cpp (9 occurrences)
- [ ] Replace std::exit() in game.cpp (12 occurrences)
- [ ] Replace std::exit() in bubble.cpp (2 occurrences)
- [ ] Update main.cpp with proper exception handling
- [ ] Extract game loop from constructor to Game::run()

### Week 3 Tasks
- [ ] Implement graceful degradation for missing assets
- [ ] Add error recovery UI components
- [ ] Create asset loading report system
- [ ] Add fallback assets (colored rectangles, default font)
- [ ] Implement retry mechanisms for transient failures

### Week 4 Tasks
- [ ] Complete unit test coverage for error handling
- [ ] Add integration tests for error scenarios
- [ ] Update documentation with error handling guide
- [ ] Performance test error handling overhead
- [ ] Code review and refinement

## Success Metrics

1. **Zero std::exit() calls** in the codebase
2. **100% exception safety** in critical paths
3. **Graceful degradation** for 80% of asset failures
4. **Unit test coverage** > 90% for error handling code
5. **No performance regression** from error handling overhead

## Long-term Benefits

1. **Improved Stability**: Proper cleanup on errors prevents resource leaks
2. **Better User Experience**: Graceful degradation instead of crashes
3. **Enhanced Debugging**: Detailed error information with context
4. **Testability**: Can unit test error scenarios
5. **Maintainability**: Clear error handling patterns

## Risk Mitigation

- **Performance Impact**: Use noexcept where appropriate, minimize exception use in hot paths
- **Binary Size**: Keep exception messages concise, use string_view
- **Compatibility**: Ensure SFML exception compatibility
- **Learning Curve**: Provide clear examples and documentation

## Conclusion

This plan transforms DeerPortal's error handling from a critical weakness to a robust system that enhances stability and user experience. The phased approach ensures minimal disruption while systematically improving code quality.

The investment in proper error handling will pay dividends in:
- Reduced bug reports
- Easier debugging
- Better user retention
- Professional code quality

Total estimated effort: 4 weeks for complete implementation and testing.