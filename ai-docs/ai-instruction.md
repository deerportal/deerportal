# 🎯 Goals

This repository contains the DeerPortal SFML game, developed by you! 🦌

Our primary objective is to migrate this project to **SFML 3.0.0** (from its current version). You can find the SFML 3.0.0 download here: https://www.sfml-dev.org/download/sfml/3.0.0/

# 🛠️ Development Guidelines

We are developing this on macOS, but the final product should also work seamlessly on Linux and Windows. 💻

We are using CMake (preferably the latest version) to build the project. ⚙️

Always adopt a **systematic approach**. 🤔 Think ahead about which other files might be affected by changes. After a maximum of **7 tool calls**, please pause to perform a systematic analysis. Look at the latest errors, and try to predict if similar issues might occur in files that haven't been compiled yet. Conduct proactive analysis every 5 such steps.

A helpful analysis of how the game works, which is beneficial for the migration, is available in `game-ai-analysis.md`. 📄

# 📖 Migration Guide

The official SFML 3.0 migration guide is available here: https://www.sfml-dev.org/tutorials/3.0/getting-started/migrate/

# 📚 Sources and Manuals (HTML)

Inside the `orig-sfml` folder, you'll find the SFML source code and HTML documentation. Please use these resources to ensure accuracy during the migration process.

# 🧠 Memory Bank

A file named `cursor-memory-bank.md` will be used as a memory bank. 💾 Please store gathered knowledge about the migration progress in this file to maintain a better context for our work.

# AI Instructions for DeerPortal SFML 3.0 Migration

## Project Overview
DeerPortal is a strategic board game with 4 players representing elemental forces (Water/Earth/Fire/Air). The project has been successfully migrated from SFML 2.x to SFML 3.0.

## Migration Status: ✅ COMPLETE
**Date Completed**: January 2025

### Major Achievements
1. ✅ **SFML 3.0 API Migration**: All breaking changes successfully addressed
2. ✅ **Asset Loading System**: Fixed double "assets/" path issue 
3. ✅ **macOS App Bundle**: Working asset loading from app bundle
4. ✅ **Compilation**: Zero errors, successful linking
5. ✅ **Runtime**: Application launches and runs successfully

## Key Technical Changes Made

### 1. Asset Path Fix (Critical Issue Resolved)
**Problem**: Double "assets/" in paths causing "Failed to open sound file" errors
- `ASSETS_PATH` already contained `"assets/"` but code added another `"assets/"`
- Result: `"assets/assets/audio/dice.ogg"` instead of `"assets/audio/dice.ogg"`

**Solution**: Systematically removed extra `"assets/"` from all asset loading calls:
- `ASSETS_PATH"assets/audio/file.ogg"` → `ASSETS_PATH"audio/file.ogg"`
- **Files Fixed**: rounddice.cpp, soundfx.cpp, game.cpp, textureholder.cpp, bubble.cpp, rotateelem.cpp, guiwindow.cpp

### 2. SFML 3.0 API Modernization
- **Default Constructors**: Converted sf::Text and sf::Sprite to use unique_ptr with font/texture initialization
- **Event System**: Migrated to variant-based event handling with `std::optional<sf::Event>`
- **Rectangle API**: Updated sf::Rect field access (.left → .position.x, etc.)
- **Position API**: Changed setPosition to use Vector2f parameters
- **Enumeration Scoping**: Updated to scoped enums (sf::Keyboard::Key::A)

### 3. Memory Management
- **Smart Pointers**: Comprehensive conversion to std::unique_ptr for SFML objects
- **Resource Safety**: Proper initialization order and null checks

## Current Status
✅ **Builds Successfully**: Zero compilation errors
✅ **Runs Successfully**: Application launches and loads assets correctly
⚠️ **Minor UI Issues**: Some visual glitches remain but do not affect core functionality

## Development Guidelines
- **Reusable Code**: Split functionalities into separate files/classes
- **Modular Design**: Keep components isolated and well-defined
- **Asset Management**: Use get_full_path() for all asset loading
- **Error Handling**: Proper validation for resource loading

## Project Structure
- **Core Game**: src/game.cpp (main game logic)
- **Asset Loading**: src/textureholder.cpp, src/soundfx.cpp
- **UI Components**: src/playerhud.cpp, src/guiwindow.cpp
- **Graphics**: src/character.cpp, src/animation.cpp
- **Audio**: src/soundfx.cpp, integrated in game.cpp

The DeerPortal SFML 3.0 migration is now complete and functional.