# 🎯 Goals

This repository contains the DeerPortal SFML game, developed by you! 🦌

**Current Version: 0.9.0 "Welcome Back"** 🎉

Our primary objective was to migrate this project to **SFML 3.0.0** (from its previous version). You can find the SFML 3.0.0 download here: https://www.sfml-dev.org/download/sfml/3.0.0/

## Migration Status: ✅ COMPLETE (Version 0.9.0 "Welcome Back")
**Date Completed**: January 2025
**Current Release**: 0.9.0 "Welcome Back" - May 2025

✅ **SFML 3.0 Migration**: Successfully migrated from SFML 2.x to SFML 3.0.0
✅ **C++17 Compatibility**: Fixed all nodiscard warnings and enhanced error handling
✅ **Version Update**: Released version 0.9.0 "Welcome Back"
✅ **Contact Updates**: Updated all email addresses to bluszcz@gmail.com
✅ **Release Notes**: Created comprehensive release documentation

## Recent Changes (Version 0.9.0 Session):

### 1. Version Release Management
- **Version Number**: Updated from 0.8.2 to 0.9.0 "Welcome Back"
- **Semantic Versioning**: Ensured consistency across all configuration files
- **Release Notes**: Created comprehensive RELEASE_NOTES.md with git history
- **AppData**: Updated application metadata for proper distribution

### 2. Contact Information Updates
- **Email Migration**: Changed all instances of bluszcz@devcarpet.net to bluszcz@gmail.com
- **Files Updated**: debian/*, documentation, and metadata files
- **Consistency**: Ensured uniform contact information across the project

### 3. SFML 3.0 C++17 Compatibility Fixes
- **Nodiscard Warnings**: Fixed two critical warnings related to `resize()` methods
  - `src/boardelem.cpp`: Added error handling for `defaultTexture.resize()`
  - `src/game.cpp`: Added error handling for `renderTexture.resize()`
- **Error Handling**: Implemented proper exception handling with `std::runtime_error`
- **Modern C++**: Enhanced code safety and compliance with C++17 standards
- **Dependencies**: Added `<stdexcept>` includes where needed

### 4. Documentation Quality
- **README.md**: Fixed grammar errors and improved clarity
- **Technical Accuracy**: Updated platform references (OSX → macOS)
- **Consistency**: Ensured professional documentation standards

## 🎮 The Game

DeerPortal is a hybrid board/card game featuring:
- **Gameplay**: 0-4 players compete to collect diamonds and survive elemental chaos
- **Theme**: Four classical elements with the Almighty Deer God
- **Mechanics**: Board movement, card actions, seasonal changes
- **Graphics**: Modern SFML 3.0 graphics engine with shaders and effects

## 🔧 Technical Stack

### Core Technologies
- **Graphics**: SFML 3.0.0 (fully migrated and compatible)
- **Language**: C++17 with modern practices
- **Build System**: CMake with cross-platform support
- **Platforms**: Linux, macOS, Windows

### Project Structure
- **Source Code**: `src/` directory with modular C++ classes
- **Assets**: `assets/` with graphics, audio, and shader resources
- **Build Configuration**: CMakeLists.txt with SFML 3.0 integration
- **Packaging**: debian/, snapcraft.yaml for distribution

## 🚀 Current Status

The project is now **production-ready** with:
- ✅ Complete SFML 3.0 migration
- ✅ Modern C++17 compatibility
- ✅ Cross-platform build system
- ✅ Professional documentation
- ✅ Clean version 0.9.0 "Welcome Back" release

All major technical debt has been resolved, and the game is ready for distribution and further development.

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

## 🎨 Asset History & Important Changes

### Background Assets Evolution
- **background_land.png White Strip Analysis**:
  - **Initial Creation**: Commit `3c946ba7130f0f1d973a31de876c3e7225312c36` (Jan 31, 2016) by Rafal Zawadzki - No white strip, dark green background
  - **White Strip Introduced**: Commit `6118565ef4689d260b1fbedfe26eaaa08520fad3` (Oct 31, 2016) by user `ukata` with message "updated background"
  - **Reference**: [GitHub Commit](https://github.com/bluszcz/deerportal/commit/6118565ef4689d260b1fbedfe26eaaa08520fad3)
  - **White Color Hex Values** (top-left area):
    - `#FBFDF8` - Very light greenish-white: `rgb(251,253,248)`
    - `#FBFCF8` - Light greenish-white: `rgb(251,252,248)`
    - `#FBFCFA` - Light white with slight green tint: `rgb(251,252,250)`
    - `#FDFEF9` - Near-white with tiny green tint: `rgb(253,254,249)`
    - `#FFFFFF` - Pure white: `rgb(255,255,255)`
  - **Note**: The white strip was added approximately 9 months after the initial background file creation and is present in all versions since October 2016.

## 🔄 **Maintenance Guidelines & Best Practices**

### **Always Update When Making Changes:**
1. **RELEASE_NOTES.md**: 
   - Document all features, fixes, and changes in each version
   - Follow semantic versioning (major.minor.patch)
   - Include dates and contributor attribution
   - Reference GitHub commits/PRs when applicable
   
2. **.gitignore**: 
   - Add generated files (.cache/, build/, compile_commands.json)
   - Include platform-specific files (.DS_Store, *.swp, etc.)
   - Exclude temporary development files
   - Update when new build tools or IDEs are used

3. **Version Consistency**:
   - Update version.txt for releases
   - Sync version numbers across CMakeLists.txt, package files
   - Update copyright years and contact information
   - Verify AppData metadata matches current version

4. **Documentation Updates**:
   - Keep README.md current with latest SFML version and dependencies
   - Update build instructions for new platforms or requirements
   - Maintain AI documentation files with latest context
   - Document API changes and migration guides

### **Pre-Commit Checklist:**
- [ ] Updated RELEASE_NOTES.md with changes
- [ ] Verified .gitignore excludes new generated files
- [ ] Checked version consistency across project files
- [ ] Updated documentation if APIs changed
- [ ] Tested build process on target platforms

# AI Instructions for DeerPortal SFML 3.0 Migration

## Project Overview
DeerPortal is a strategic board game with 4 players representing elemental forces (Water/Earth/Fire/Air). The project has been successfully migrated from SFML 2.x to SFML 3.0.

## Migration Status: ✅ COMPLETE (Version 0.9 "Welcome Back")
**Date Completed**: January 2025
**Current Release**: 0.9 "Welcome Back" - May 2025

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