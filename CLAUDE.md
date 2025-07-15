# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

DeerPortal is a hybrid board/card game built with SFML 3.0 and C++17. It's a cross-platform game supporting 0-4 players with AI opponents, featuring elemental mechanics and card-based gameplay.

**Current Version: 0.9.3 "CI/CD Pipeline Enhancement"**  
**Technology Stack: SFML 3.0.1, CMake, C++17**  
**Code Quality: A Grade (91/100) - Professional implementation**

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
src/                 # Main C++ source code (~60 files, ~6,807 lines)
assets/             # Game assets (images, audio, fonts, shaders)
packaging/          # Platform-specific packaging resources
ai-docs/           # AI assistant documentation and context
scripts/           # Build and utility scripts
cmake_modules/     # CMake helper modules
```

## Development Guidelines

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

### 0.9.2-pre.1 Preparation
- LICENSE now included in Windows installer and all packaging systems.
- Version updated to 0.9.2-pre.1.
- Improved systematic approach for packaging files.

### Windows Build Success (2025-07-13)
- ✅ **MAJOR SUCCESS:** Windows build now working with VS 2022 + SFML 3.0.1
- ✅ **Fresh VS 2022 installation** resolved all CMake generator issues
- ✅ **SFML 3.0.1 built from source** successfully (CI/CD approach)
- ✅ **DeerPortal.exe compiles and runs** - all game systems functional
- ✅ **Confirmed working command sequence** documented in windows-troubleshooting.md
- ✅ **BREAKTHROUGH: Asset packaging FIXED** - POST_BUILD commands working perfectly
- ✅ **Automatic asset copying** - no more manual intervention needed
- ✅ **LICENSE file inclusion** - properly configured in CMake and qmake
- ✅ **Installation process verified** - cmake --install works flawlessly
- ✅ **NSIS installer license display** - FIXED with CPACK_RESOURCE_FILE_LICENSE
- ✅ **Start Menu integration** - FIXED with CPACK_NSIS_CREATE_ICONS_EXTRA
- ✅ **Windows search visibility** - DeerPortal now appears in Start Menu search
- 🎯 **Status:** 0.9.2-pre.1 objectives COMPLETED - ready for release

## Recent Updates

### Version 0.9.3-pre.5 Updates
- ✅ **CI/CD Artifact Upload Fix**: Fixed conditional logic in GitHub Actions workflows preventing TGZ and ZIP artifacts from reaching releases
- ✅ **Input Test Mode Handling**: Corrected `!inputs.test_mode` to `(inputs.test_mode != true)` for proper tag-triggered builds
- ✅ **Artifact Upload Reliability**: Ensured all package formats (TGZ, ZIP, EXE, DEB, DMG, AppImage) reach GitHub releases consistently
- ✅ **Conditional Evaluation Bug Fix**: Resolved undefined `inputs.test_mode` causing upload steps to be skipped in tag-based releases
- ✅ **Cross-Platform Package Delivery**: Enhanced upload reliability across all platforms and package formats

### Version 0.9.3-pre.4 Updates
- ✅ **AppImage Icon Path Fix**: Fixed AppImage icon path issues that prevented AppImage creation in CI/CD
- ✅ **Icon File Path Correction**: Corrected paths from non-existent net.devcarpet.deerportal.png to actual DeerPortal.iconset/icon_256x256.png
- ✅ **AppImage Creation Robustness**: Enhanced with comprehensive icon fallback strategies and multiple detection methods
- ✅ **Desktop File Compliance**: Resolved "icon defined in desktop file but not found" error in AppImage workflow

### Version 0.9.3-pre.3 Updates
- ✅ **FUSE Group Creation Fix**: Resolved "usermod: group 'fuse' does not exist" error in GitHub Actions workflows
- ✅ **Automatic Group Management**: Added fuse group creation with groupadd when group doesn't exist
- ✅ **Immediate Group Changes**: Enhanced group management with newgrp for immediate group membership application
- ✅ **Comprehensive FUSE Setup**: Improved FUSE setup reliability for AppImage creation across all CI/CD pipelines
- ✅ **Enhanced Debugging**: Added detailed FUSE group membership verification and status reporting

### Version 0.9.3-pre.2 Updates
- ✅ **FUSE Installation and Configuration**: Resolved AppImage creation failures in GitHub Actions workflows
- ✅ **Mandatory FUSE Setup**: Added comprehensive FUSE support with modprobe, permissions, and verification
- ✅ **Linux Artifact Reliability**: Enhanced AppImage creation with guaranteed FUSE dependencies
- ✅ **CI/CD Pipeline Robustness**: Eliminated fallback workarounds in favor of proper FUSE support
- ✅ **Workflow Reliability**: Comprehensive FUSE setup applied to both release and test-release workflows

### Version 0.9.3-pre.1 Updates
- ✅ **Intro Mouse Click Skip Fix**: Fixed GameInput module to properly handle intro skipping with mouse clicks
- ✅ **CI/CD Artifact Upload Reliability**: Enhanced file detection and error handling for TGZ and ZIP uploads
- ✅ **Fullscreen Toggle Improvements**: Robust cross-platform fullscreen functionality with F/F11 keys
- ✅ **Context-Aware Escape Key**: Proper Game→Menu→Exit behavior with unified input system
- ✅ **Comprehensive Workflow Debugging**: Enhanced CI/CD pipelines with robust file detection and continue-on-error

### Version 0.9.2 Updates
- ✅ **Computer Player Card Notification Delay**: Implemented 4-second delay for computer player card notifications
- ✅ **Enhanced Game Flow**: Human players now have sufficient time to read card messages before computer continues
- ✅ **User Experience Improvement**: Fixed rapid computer turn progression issue in turn-based gameplay
- ✅ **Timer System Integration**: Added cardNotificationDelay system integrated with existing AI behavior

### Version 0.9.2-pre.2 Updates
- Enhanced Windows build process with SFML 3.0.1
- Documented PowerShell execution policy workarounds
- Fixed grid reveal intro shader loading confirmation
- Created detailed fullscreen implementation plan (F key toggle)

### Recent Improvements (July 2025)

#### Architecture Enhancements
- **Input System Unified**: Centralized event processing through GameInput module
  - Fixed F11/F key fullscreen toggle reliability
  - Implemented context-aware Escape behavior (Game→Menu→Exit)
  - Resolved mouse click issues and event conflicts

#### Performance Optimization
- **V-Sync Implementation**: Eliminated conflicting frame rate settings
  - CPU usage reduced from 75% to 15-25%
  - Adaptive refresh rate support (60Hz/120Hz/144Hz)
  - Smooth gameplay with eliminated screen tearing

#### User Experience Improvements
- **Card Notifications**: Unified 4-second delay for all player types
  - Human and computer players now have equal notification viewing time
  - Prevents accidental notification skipping
  - Consistent game flow across all interaction modes

#### Distribution Enhancements
- **AppImage Support**: Added universal Linux distribution format
  - APPDIR environment detection for portable execution
  - Standard Linux application structure (`/usr/share/deerportal/`)
  - Third distribution method alongside DEB and TGZ packages

### Current Features (Implemented)
- **Fullscreen Mode**: Multi-key fullscreen toggle (F, F11, Cmd+Enter)
  - Professional SFML 3.0.1 window recreation pattern
  - State preservation across fullscreen transitions
  - Cross-platform compatibility with error recovery

## Build and Development

When working on github actions fixes, always check release among with test release if they are unified.
