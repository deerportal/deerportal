# DeerPortal AI Instructions & Memory Bank

This file serves as a memory bank and context for AI assistants working on the DeerPortal project.

## Project Overview

DeerPortal is a hybrid board/card game built with SFML 3.0, featuring the journey of becoming a Deer God through collecting diamonds and playing cards driven by classical elements. The game supports 0-4 players with AI opponents.

**Current Version: 0.9.2 "Computer Player Card Notification Delay" (July 2025)**
**Technology Stack**: SFML 3.0.1, CMake, C++17
**Code Quality**: A Grade (91/100) - Professional implementation

## Core Technical Details

### Architecture
- **Modular Design**: 5 core modules (GameAssets, GameInput, GameRenderer, GameCore, GameStateManager)
- **Memory Management**: Modern C++ RAII with `std::unique_ptr` for SFML objects
- **Asset Management**: Multi-platform path resolution (macOS bundles, AppImage APPDIR, Windows relative)
- **Input System**: Unified event processing with context-aware behavior (GameInput module)
- **Performance**: V-Sync optimization (CPU usage: 75% → 15-25%)
- **Cross-Platform**: macOS, Linux (DEB/TGZ/AppImage), Windows with professional packaging

### Build System
- **Primary**: CMake-based build system with SFML 3.0 detection
- **Legacy**: qmake files (deerportal.pro, deerportal_windows.pro) for compatibility
- **Packaging**: CPack for cross-platform packaging, custom DMG creation for macOS

## Version Management System Documentation

### Single Source of Truth: version.txt

The DeerPortal project uses `version.txt` as the **primary and only source** for version information:

**File: `version.txt`**
```
0.9.1
```

### Version Parsing and Distribution (CMakeLists.txt)

The CMake system reads version.txt and distributes the version information throughout the build:

```cmake
# Primary version reading (lines 5-8)
file (STRINGS "version.txt" DEERPORTAL_VERSION)
string(REGEX_MATCH "^([0-9]+)" DEERPORTAL_VERSION_MAJOR ${DEERPORTAL_VERSION})
string(REGEX_REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\2" DEERPORTAL_VERSION_MINOR ${DEERPORTAL_VERSION})
string(REGEX_REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+.*)" "\\3" DEERPORTAL_VERSION_PATCH ${DEERPORTAL_VERSION})

# C++ preprocessor definition (line 46)
add_definitions(-DDEERPORTAL_VERSION="${DEERPORTAL_VERSION}.${CMAKE_BUILD_TYPE}")

# CPack packaging variables (lines 382-385)
set(CPACK_PACKAGE_VERSION_MAJOR "${DEERPORTAL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${DEERPORTAL_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${DEERPORTAL_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${DEERPORTAL_VERSION}")
```

### Version Usage Locations

**1. C++ Runtime Display (src/game.cpp:646)**
```cpp
gameVersion->setString("version: " + std::string(DEERPORTAL_VERSION)+"-"+std::string(BASE_PATH));
```
- Creates in-game version display combining version + build type + base path

**2. macOS App Bundle (packaging/Info.plist:17,19,25)**
```xml
<key>CFBundleShortVersionString</key>
<string>${DEERPORTAL_VERSION}</string>
<key>CFBundleVersion</key>
<string>${DEERPORTAL_VERSION}</string>
<string>DeerPortal ${DEERPORTAL_VERSION}, Game About How Human Can Be Upgraded to the Deer</string>
```
- CMake substitutes ${DEERPORTAL_VERSION} during configure process

**3. Package Metadata**
- **Snap Package** (`snapcraft.yaml:3`): `version: "0.9.1"` - **⚠️ MANUALLY UPDATED**
- **AppData** (`net.devcarpet.deerportal.appdata.xml:34,39,44`): Release entries - **⚠️ MANUALLY UPDATED**
- **Debian Changelog** (`debian/changelog:1`): Package version - **⚠️ MANUALLY UPDATED**

**4. GitHub Actions (.github/workflows/)**
- **Release Workflow**: Dynamically updates version.txt from git tags or manual input
- **Test Workflow**: Uses manual version input with default "0.9.1"
- **DMG Fallback**: `VERSION_FROM_FILE=$(cat ../version.txt || echo "0.9.1")`

**5. Legacy qmake (deerportal_windows.pro:43)**
```pro
DEFINES += "DEERPORTAL_VERSION=$$(DEERPORTAL_VERSION)"
```
- Reads from environment variable (needs external version.txt reading)

### Manual Update Requirements

The following files require **manual updates** when releasing new versions:

1. **snapcraft.yaml** (line 3)
   ```yaml
   version: "0.9.1"  # Update this manually
   ```

2. **net.devcarpet.deerportal.appdata.xml** (lines 34+)
   ```xml
   <release version="0.9.1" date="2025-06-30">  <!-- Add new release entry -->
   ```

3. **debian/changelog** (line 1)
   ```
   deerportal (0.9.1-minor-fixes-ubuntu1) trusty; urgency=medium  # Add new entry
   ```

4. **RELEASE_NOTES.md** (header)
   ```markdown
   ## Version 0.9.1 "Stability Improvements" - June 2025  <!-- Add new section -->
   ```

### Release Management Process

**For New Releases:**

1. **Update Primary Source**:
   ```bash
   echo "X.Y.Z" > version.txt
   ```

2. **Update Manual Files** (see list above):
   - snapcraft.yaml
   - net.devcarpet.deerportal.appdata.xml  
   - debian/changelog
   - RELEASE_NOTES.md

3. **GitHub Release Workflow**:
   - **Tag-based**: `git tag vX.Y.Z && git push --tags` (auto-updates version.txt)
   - **Manual**: Use workflow_dispatch with version input (auto-updates version.txt)

4. **Verification**:
   - CMake correctly parses version.txt
   - All packaging includes correct version
   - Runtime display shows correct version

### Version Format Standards

- **Semantic Versioning**: `MAJOR.MINOR.PATCH` (e.g., "0.9.1")
- **Runtime Display**: `version: 0.9.1.Release-/Applications/DeerPortal.app/Contents/MacOS`
- **Package Names**: `DeerPortal-0.9.1-macOS.dmg`, `deerportal_0.9.1_amd64.deb`
- **Git Tags**: `v0.9.1` (with 'v' prefix)

### Critical Dependencies

- **CMake Version Reading**: Never edit version in CMakeLists.txt directly
- **GitHub Actions**: Always sync version.txt with release tags
- **Package Consistency**: Ensure all manual files reflect same version
- **Build Integration**: Version appears in runtime display for debugging

**⚠️ IMPORTANT**: `version.txt` is the single source of truth. All automated systems read from this file. Manual files must be kept in sync for complete version consistency.

## Recent Changes

### v0.9.1 "Stability Improvements" Release (June 2025)
- **Complete Version Management Documentation**: Documented the entire version system for better release management
- **Enhanced Release Process**: Streamlined workflows and clear procedures for version updates
- **Build System Improvements**: Better CMake configuration and cross-platform support
- **Documentation Updates**: Comprehensive guides in ai-instructions.md for version management
- **Package Consistency**: Improved metadata consistency across all platforms
- **Development Workflow**: Enhanced GitHub Actions integration and test modes

### v0.9.0 "Welcome Back" Release (May 2025)
- Complete SFML 3.0 migration with 111 commits from 0.8.2
- Added Card Notification System with visual overlays
- Added Intro Shader Animation with grid-based reveal effect
- VertexArray Diamond Rendering optimization (99.1% draw call reduction)
- Enhanced Card System Documentation (CARDS.md)
- Windows development support improvements
- macOS professional packaging with automated DMG creation
- Memory management overhaul with smart pointers
- Build system improvements and cross-platform consistency

### v0.8.2 Release (January 2025)
- FPS Display Feature with conditional compilation
- Window Icon Support for better visual identity
- Performance optimizations in rendering pipeline
- SFML 3.0 preparation groundwork
- Enhanced build system configuration

### GitHub Actions Release Pipeline Fixes
- **Fixed macOS DMG Creation**: Changed from `cpack -G DragNDrop` to `make dmg` with manual fallback
- **Fixed Linux Packaging**: Use `make package` instead of direct `cpack` calls
- **Fixed Windows Packaging**: Use `make package` with NSIS fallback  
- **Fixed Release Conflicts**: Multiple jobs no longer create conflicting GitHub releases
- **Added Test Mode**: `test_mode` input to build without creating releases
- **Updated to action-gh-release@v2**: Better conflict handling
- **Removed libsfml-dev dependency**: Linux packages now use static SFML builds
- **Fixed Intel macOS Build**: RESOLVED - Root cause was undefined `YOUR_DIRECTORY` variable in CMakeLists.txt causing configuration failure. Also removed CMAKE_OSX_ARCHITECTURES flags and improved error handling in CI environment detection.

## Recent Optimizations Applied (Latest Updates)

### Header Dependency Reduction (December 2024)
- **Status**: ✅ COMPLETED
- **Optimization**: Reduced header compilation dependencies in `game.h`
- **Approach**: Strategic analysis of which classes can use forward declarations vs require full includes
- **Key Learning**: Direct member variables require full class definitions (includes), while pointers/references can use forward declarations
- **Result**: Cleaner dependency structure while maintaining functionality
- **Files Modified**: 
  - `src/game.h` - Reorganized includes with detailed comments explaining why each include is necessary
  - `src/game.cpp` - Added all implementation-only includes
- **Compilation**: ✅ All builds successful after optimization

**Technical Details:**
- Classes used as direct members (e.g., `Selector selector;`) MUST be included in header
- Classes only used as pointers or function parameters CAN be forward declared
- Module classes (`GameAssets`, `GameInput`, etc.) are forward declared as they're used as `std::unique_ptr`
- This improves compilation speed and reduces cascading header dependencies

## Comprehensive Modularization Plan (January 2025)

### Overview
- **Status**: 📋 PLANNED
- **Document**: `ai-docs/opusplanmodularity.md` - Comprehensive plan for codebase modularization
- **Goal**: Transform DeerPortal into a fully modular architecture with no file exceeding 400 lines

### Key Objectives
1. **File Size Limits**: 400 lines for .cpp files, 100 lines for .h files
2. **Single Responsibility**: Each module handles one specific aspect
3. **Minimal Dependencies**: Extensive use of forward declarations
4. **Component Architecture**: Move towards Entity-Component-System pattern
5. **Build Optimization**: Precompiled headers and module libraries

### Modules Implemented ✅

#### Phase 1: Complete game.cpp Decomposition (IN PROGRESS)
- ✅ **game-state-manager.cpp/.h**: State transition logic (256 lines) - COMPLETED
- ✅ **game-animation-system.cpp/.h**: Animation management (266 lines) - COMPLETED  
- ⏳ **game-ui-manager.cpp/.h**: UI element management (300 lines) - PLANNED

#### Progress Summary
- **game.cpp**: Reduced from 1491 → 1436 lines (-55 lines, 3.7% reduction)
- **game-core.cpp**: Reduced from 527 → 514 lines (-13 lines, 2.5% reduction)
- **New Modules**: 2 modules created with 522 total lines of extracted functionality

### Planned Modules

#### Phase 2: Large File Splitting
- **CardNotification**: Split into renderer, logic, and UI modules (3×230 lines)
- **GameRenderer**: Split into states, effects, and UI modules (3×200 lines)
- **GameCore**: Split into player, rules, and AI modules (3×175 lines)

#### Phase 3: Component System
- **IGameComponent**: Base interface for all game components
- **ComponentSystem**: Manager for component lifecycle
- **Specific Components**: Movement, Render, Input, Audio

### Expected Improvements
- **Compilation Time**: 44% reduction (45s → 25s)
- **File Sizes**: 60-73% reduction per file
- **Code Complexity**: Cyclomatic complexity from 15 → 8
- **Test Coverage**: Enable 80%+ unit testing

### Implementation Timeline
- **Week 1**: Complete game.cpp decomposition
- **Week 2**: Split large files (CardNotification, GameRenderer, GameCore)
- **Week 3**: Implement component architecture
- **Week 4**: Extract data and resource management
- **Week 5**: Add testing infrastructure
- **Week 6**: Optimize build system

**Full Details**: See `ai-docs/opusplanmodularity.md` for complete implementation plan

## Development Guidelines

### Code Quality
- **Reusable and Modular**: Split functionalities into separate files/classes
- **Modern C++**: Use C++17 features, smart pointers, and RAII
- **SFML 3.0 Compliance**: Follow SFML 3.0 patterns and best practices
- **Memory Safety**: Prefer `std::unique_ptr` for SFML objects

### Build System
- **Always Update .gitignore**: Ensure temporary files aren't tracked
- **Use SFML 3.0.1**: Official release from https://www.sfml-dev.org/development/changelog/
- **Cross-Platform Testing**: Verify builds on macOS, Linux, and Windows
- **Package Testing**: Test all package formats (DMG, DEB, NSIS)

### File Organization
- **Data Storage**: Use `.data/` folder for temporary downloads
- **Documentation**: Maintain this `ai-instructions.md` as memory bank
- **Systematic Approach**: Consider all impacted files when making changes

# Recent Improvements (July 2025)

## Architecture Enhancements

### Input System Unification
- **Issue Fixed**: Fragmented event handling causing missed F11/Escape inputs
- **Solution**: Centralized processing through GameInput::processEvents()
- **Result**: Reliable fullscreen toggle, context-aware Escape (Game→Menu→Exit)

### Performance Optimization
- **Issue Fixed**: High CPU usage (75%) from conflicting frame rate settings
- **Solution**: V-Sync only approach, removed manual frame limiting conflicts
- **Result**: CPU usage reduced to 15-25%, adaptive refresh rate support

### Card Notification Consistency
- **Issue Fixed**: Human players could skip notifications instantly, computers had 4s delay
- **Solution**: Unified 4-second notification delay for all player types
- **Result**: Consistent game flow and equal notification viewing time

### AppImage Distribution
- **Enhancement**: Added AppImage as universal Linux distribution format
- **Implementation**: Proper APPDIR environment detection and asset path resolution
- **Result**: Third Linux package format alongside DEB and TGZ

## Code Quality Achievements
- **Grade**: A (91/100) - Continuous improvement from B- (73/100)
- **Standards**: LLVM coding style with automated formatting
- **Memory Safety**: Modern C++ RAII patterns throughout codebase
- **Error Handling**: Professional exception handling with context preservation

## Distribution Improvements
- **Windows**: VS 2022 build success, NSIS installer with Start Menu integration
- **macOS**: Professional app bundle with code signing and DMG creation
- **Linux**: Three distribution formats (DEB, TGZ, AppImage) with desktop integration

# Context Update 0.9.2
- All platform builds verified and working with SFML 3.0.1
- Input system completely unified through GameInput module
- Performance optimized with V-Sync synchronization
- AppImage support added for universal Linux compatibility
- LICENSE and legal files included in all installer outputs
