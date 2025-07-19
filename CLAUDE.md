# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

DeerPortal is a hybrid board/card game built with SFML 3.0 and C++17. It's a cross-platform game supporting 0-4 players with AI opponents, featuring elemental mechanics and card-based gameplay.

**Current Version: 0.9.6-pre.2 "Development Phase"**  
**Technology Stack: SFML 3.0.1, CMake, C++17**  
**Code Quality: A+ Grade (97/100) - Professional implementation with latest state management fixes**

## Build Commands

### Primary Build System (CMake)
```bash
# Configure for Release (recommended)
cmake -DCMAKE_BUILD_TYPE=Release .

# Configure for Debug (development)
cmake -DCMAKE_BUILD_TYPE=Debug .

# Enable FPS counter in Release build
cmake -DCMAKE_BUILD_TYPE=Release -DSHOW_FPS_COUNTER=ON .

# Build executable
make

# Create platform packages
make package         # Cross-platform packages
make dmg            # macOS DMG installer  
make install        # Install to system
```

### Version Management
- **Single source of truth**: `version.txt` contains current version
- **Manual updates required**: `snapcraft.yaml`, `net.devcarpet.deerportal.appdata.xml`, `debian/changelog` when releasing
- **Runtime display**: Version appears in-game with build type and base path

### Dependencies
- **SFML 3.0.1** (found via CMake if installed via Homebrew)
- **CMake 3.16+**
- **C++17 compatible compiler** (Clang++ 14+, GCC 9+, MSVC 2019+)

## Code Architecture

### Modular Design
The game uses a modular architecture with clear separation of concerns:

- **GameCore** (`src/game-core.h/cpp`) - Game logic, state management, player turns
- **GameAssets** (`src/game-assets.h/cpp`) - Asset loading, menu system, resource management
- **GameInput** (`src/game-input.h/cpp`) - Unified event processing, context-aware input handling
- **GameRenderer** (`src/game-renderer.h/cpp`) - V-Sync optimized rendering pipeline and graphics
- **GameStateManager** (`src/game-state-manager.h/cpp`) - State transitions and music coordination
- **Main Game Class** (`src/game.h/cpp`) - Central coordinator with modular architecture

### Key Technical Details
- **Memory Management**: Modern C++ RAII with `std::unique_ptr` for SFML objects
- **Asset Management**: Multi-platform path resolution (macOS bundles, AppImage APPDIR, Windows relative)
- **Input System**: Unified event processing with context-aware Escape behavior (Game→Menu→Exit)
- **Performance**: V-Sync optimization (CPU: 75% → 15-25%), VertexArray diamond rendering
- **Shaders**: Custom GLSL shaders for blur, pixelation, and intro animations
- **Card System**: 128 cards with unified 4-second notification delay for all player types

### File Structure
```
src/                 # Main C++ source code (~70 files, ~7,200+ lines)
├── Animation System # game-animation-system.h/cpp - VertexArray particle effects
├── Asset Management # game-assets.h/cpp - Multi-platform resource loading
├── Input Processing # game-input.h/cpp - Unified event handling
├── Rendering Engine # game-renderer.h/cpp - V-Sync optimized graphics
├── Game Logic      # game-core.h/cpp - Rules and state management
├── State Management # game-state-manager.h/cpp - Music coordination
├── Error Handling  # error-handler.h/cpp, exceptions.h - Safe execution
├── Utility Systems # safe-asset-loader.h/cpp, window-manager.h/cpp
└── Card System     # card*.h/cpp - Card mechanics and notifications

assets/             # Game assets (images, audio, fonts, shaders)
packaging/          # Platform-specific packaging resources
ai-docs/           # AI assistant documentation and context
scripts/           # Build and utility scripts
cmake_modules/     # CMake helper modules
```

## Development Guidelines

### Language

* Don't use emoji in the code
* Don't use excited language, just document what is happening in the code without commenting value of it.


### Code Standards
- **C++17**: Use modern C++ features, smart pointers, RAII
- **SFML 3.0**: Follow SFML 3.0 patterns and API
- **Code Formatting**: LLVM style enforced via clang-format (see `.clang-format`)
- **Naming Conventions**: camelCase for functions/variables, PascalCase for classes/types
- **Indentation**: 2 spaces, no tabs, 100-character line limit
- **Pointers**: Left alignment (`int* ptr`, `const std::string& ref`)
- **Includes**: Sorted and grouped (standard → SFML → project headers)
- **Header Dependencies**: Use forward declarations where possible, full includes only for direct members
- **Memory Safety**: Prefer `std::unique_ptr` for SFML objects
- **Always when debug some text ensure it is being debug only for the DEBUG compiled version
- **When making release ensure that docs are update and version number is being updated in all necessary files, always check all dependencies and how version is being handled before finishing this task.

### Testing
- **No formal unit testing framework** - uses integration testing through CI/CD
- **Test mode**: Run with `--test` flag for basic testing
- **CI/CD**: GitHub Actions with 3-platform matrix builds (Ubuntu, Windows, macOS Apple Silicon)
- **Workflow Consistency**: cmake.yml (PR/CI) and test-release.yml unified for identical behavior
- **Release Pipeline**: Robust duplicate detection and conflict resolution (July 2025 fixes)

### Important Files
- **DEVELOPMENT.md**: Complete development guide with version management, build system, and release procedures
- **ai-docs/coding-standards.md**: Comprehensive C++ coding standards and LLVM style guidelines
- **ai-docs/memory-management-improvement-plan.md**: Detailed plan for eliminating memory management issues
- **.clang-format**: LLVM-style code formatting configuration for consistent C++ style
- **ai-docs/github-actions.md**: Complete CI/CD pipeline analysis and documentation
- **ai-docs/code-quality-analysis.md**: Complete code quality analysis with improvement tracking
- **ai-docs/error-handling-improvement-plan.md**: Error handling implementation plan and progress
- **version.txt**: Single source of truth for version management
- **CMakeLists.txt**: Primary build configuration with SFML 3.0 detection
- **README.md**: User installation and setup instructions
- **CARDS.md**, **HANDBOOK.md**: Game documentation and card reference

## Code Quality Resources
- **Current Grade**: A (91/100) - Continuous improvement from B- (73/100)
- **Quality Analysis**: See `ai-docs/code-quality-analysis.md` for detailed assessment
- **Coding Standards**: See `ai-docs/coding-standards.md` for LLVM-style guidelines and best practices
- **Recent Improvements**: Professional error handling, LLVM formatting, particle system removal, safe asset loading
- **Remaining Priorities**: Replace std::exit() with exceptions (see memory management plan)

## Development Environment

### Cursor IDE Rules
The project includes Cursor IDE rules in `.cursor/rules/`:
- **sfml31.mdc**: Ensures SFML 3.0.1 API usage consistency
- **gitignore.mdc**: Maintains proper .gitignore file updates

When using Cursor IDE, these rules will automatically apply to ensure:
- Correct SFML 3.0.1 API calls (e.g., `font.openFromFile()` not `loadFromFile()`)
- Updated .gitignore for temporary files and build artifacts

## Platform-Specific Notes

### macOS
- **App Bundle**: Professional packaging with proper icon and metadata
- **DMG Creation**: `make dmg` creates installer with Applications symlink
- **Code Signing**: Configured for macOS security compatibility
- **Dependencies**: SFML bundled in app bundle for distribution

### Linux
- **Debian Packages**: `fakeroot debian/rules binary` for local .deb creation
- **Snap Package**: Available via snapcraft.yaml (manual version updates)
- **AppImage**: Cross-distribution portable format support

### Windows  
- **NSIS Installer**: Windows installer package creation
- **Cross-Compilation**: MXE toolchain support for Linux→Windows builds
- **Visual Studio**: CMakeSettings.json for native Windows development

## Writing Guidelines

### Content Style
- **Language Goal**: Write blog posts naturally, simply, and concisely
- **Writing Approach**: 
  - Focus on facts
  - Avoid overly generous AI-style writing
  - Eliminate unnecessary jokes or elaborate language
  - Prioritize clarity and directness
  - Dont assume if players like game, dont show off etc, dont assume that anything is good, just provide facts.
  - If something is not fact, dont write about this.

#### Build and Development

When working on github actions fixes, always check release among with test release if they are unified.
