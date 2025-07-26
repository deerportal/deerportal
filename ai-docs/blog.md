# DeerPortal Development Blog

## July 26, 2025 - Lighting Effects Development (v0.10.0-pre.1) - PRE-RELEASE

**Foundation Release**: Comprehensive planning and technical analysis for dynamic lighting effects during board initialization animation.

**Documentation System Enhancements**:
- **Technical Analysis Framework**: Created comprehensive final documentation covering particles, game states, sprites, animation systems, music, and sound
- **SFML 3.0.1 Source Integration**: Downloaded and integrated SFML 3.0.1 source code for technical validation and implementation guidance
- **Multi-Plan Analysis**: Analyzed three lighting implementation approaches (GROK4 shader-based, GEMINI25pro light maps, OPUS4 multi-layered)
- **Optimal Implementation Selection**: GEMINI25pro light map approach selected based on SFML 3.0.1 source validation

**Technical Foundation Work**:
- **SFML Capability Validation**: Confirmed BlendAdd, BlendMultiply, RenderTexture, and shader uniform support in SFML 3.0.1
- **Performance Architecture**: Established lighting system requirements for 60+ FPS with 112 light sources
- **Implementation Roadmap**: 5-day implementation timeline with specific technical milestones
- **Memory Optimization**: Single RenderTexture approach for minimal overhead (~4MB at screen resolution)

**Lighting System Planning**:
- **Darkness Overlay**: Semi-transparent black overlay during board initialization
- **Dynamic Light Sources**: Each diamond emits soft light radius with customizable intensity and color
- **Native GPU Blending**: Uses SFML's predefined BlendAdd and BlendMultiply for optimal performance
- **Scalable Architecture**: Light map technique scales O(1) with light count

This pre-release establishes the technical foundation for implementing spectacular lighting effects that will enhance the board initialization animation with professional visual quality while maintaining DeerPortal's performance standards.

## July 19, 2025 - Spectacular Particle Effects (v0.9.7-pre.2)

Made all particle effects dramatically more visible and spectacular. Enhanced all parameters for maximum visual impact:

**Particle Count Doubles:** DIAMOND_BURST (6→12), CARD_COLLECT (4→8), STOP_CARD (8→16), diamond x2 (10→20)

**Speed Increases:** DIAMOND_BURST (120→180), CARD_COLLECT (80→140), STOP_CARD (60→100), diamond x2 (140→220)

**Scale Increases:** DIAMOND_BURST (0.5→0.8), CARD_COLLECT (0.3→0.6), STOP_CARD (0.4→0.7), diamond x2 (0.8→1.0)

**Lifetime Extensions:** DIAMOND_BURST (1.2→1.8s), CARD_COLLECT (0.8→1.4s), STOP_CARD (1.0→1.6s), diamond x2 (1.8→2.2s)

Removed unused hardcoded radius limitation. All collection effects now provide highly visible, long-lasting spectacular visual feedback.

**Technical Documentation Updates:**
- Enhanced particle-system-optimization-plan.md with current implementation details
- Created linux-asset-path-resolution.md documenting cross-platform asset loading
- Updated CLAUDE.md with latest architectural features and technical improvements

## July 19, 2025 - Linux Distribution Fixes (v0.9.6)

Fixed critical Linux distribution issues affecting AppImage and tar.gz packages. Updated asset path resolution in filetools.h to properly locate assets in `/usr/share/games/deerportal/` for both AppImage and standard Linux installations. Modified CI/CD workflows (cmake.yml, test-release.yml, release.yml) to install assets in correct location. Added Linux-specific binary detection logic to resolve assets relative to executable location.

## July 19, 2025 - Version 0.9.6 Release - Performance and Architecture

Version 0.9.6 stable release completed with comprehensive documentation updates and architecture improvements. Updated all version files from 0.9.6-pre.2 to stable 0.9.6 across version.txt, debian/changelog, snapcraft.yaml, and appdata.xml. Enhanced modular system architecture documentation and synchronized project files.

## July 19, 2025 - Documentation Synchronization (v0.9.6-pre.2)

Updated project documentation to reflect current codebase state. Key changes include removal of legacy particle system files, addition of new utility systems (window-manager, safe-asset-loader, error-handler), and updated file structure documentation. The modular architecture improvements have been properly documented with current file counts and system organization.

## July 25, 2025 - Animated Board Initialization (v0.9.9) - STABLE RELEASE

**Major Feature Release**: Professional animated board initialization system that transforms game startup experience.

**Core Animation Features**:
- **112 Diamond Animation**: All diamonds spawn from four screen corners and smoothly animate to final board positions
- **Bézier Curve Movement**: Natural curved animation paths for visually appealing motion
- **Rotation & Scaling Effects**: Diamonds rotate and scale during animation for dynamic visual impact
- **Quadrant-Based Spawning**: Player-specific corner spawning (Player 0: top-left, Player 1: top-right, etc.)
- **Pixel-Perfect Positioning**: Seamless transition to static diamonds with exact position alignment

**Technical Achievements**:
- **Performance Optimization**: Single VertexArray draw call for all 112 animated diamonds
- **Cross-Platform Compatibility**: Linux build fixes with proper math headers and M_PI definition
- **LLVM Code Formatting**: Complete codebase formatted with consistent LLVM style standards
- **Debug System Optimization**: Unified debug macros using `#ifndef NDEBUG` standard
- **State Management Integration**: New `state_board_animation` seamlessly integrated with existing game flow

**User Experience Enhancements**:
- **Skip Functionality**: Space, Enter, or mouse click to skip animation
- **Staggered Timing**: 0.05s delays create wave-like visual effect
- **Visual Feedback**: Rotation ends at 0° to match static diamonds perfectly
- **Multiple Input Methods**: Keyboard and mouse support for accessibility

**Quality Improvements**:
- **Memory Management**: Proper RAII with unique_ptr for SFML objects
- **Error Handling**: Graceful degradation with fallback to instant placement
- **Code Documentation**: Comprehensive technical documentation in ai-docs/
- **Professional Implementation**: A+ grade code quality with latest architectural improvements

This release represents a significant milestone in DeerPortal's visual evolution, providing spectacular board awakening animations while maintaining the game's performance and stability standards.

# DeerPortal Version 0.9.5 - Code Formatting and Quality (STABLE RELEASE)

**Release Date**: July 19, 2025  
**Platforms**: Windows, macOS, Linux

## Version 0.9.5 Final Release

**Code Quality and Formatting**
- LLVM code formatting applied to entire codebase for consistency
- Comprehensive version management across all packaging files
- Code quality improvements and standardization
- Enhanced development workflow with unified formatting standards

**Critical Bug Fix: Game State Reset**
- Fixed game state persistence bug when returning from menu via Escape key
- Players pressing Escape→Menu→Play now get properly reset game state
- Previously: players/resources/positions persisted inappropriately
- Solution: Added `game->restartGame()` call in `GameStateManager::showGameBoard()`
- Simple one-line fix ensures clean game restart experience

**Version Management**
- Updated from 0.9.5-pre.1 to stable 0.9.5 across all files
- Unified version handling in debian/changelog, snapcraft.yaml, appdata.xml
- Single source of truth maintained in version.txt

**Technical Improvements**
- Enhanced game state management and transition reliability
- Improved user experience with consistent game restart behavior
- Maintained all features from previous versions while fixing critical state issue

# DeerPortal Version 0.9.1 - Stability Improvements (FINAL RELEASE)

**Release Date**: July 14, 2025  
**Platforms**: Windows, macOS, Linux

## Version 0.9.1 Final Updates

This final release delivers comprehensive CI/CD pipeline improvements and automated documentation generation.

**GitHub Actions CI/CD Overhaul**
- Scout+Warrior release pattern eliminates race conditions
- Smart upload system with conditional file detection
- Enhanced cross-platform reliability and debugging
- Professional packaging across all platforms with consistent naming

**Automated Documentation Generation**
- PDF handbook generation using Pandoc and LaTeX
- Version-specific documentation included in every release
- Enhanced release notes with comprehensive installation instructions

**Workflow Reliability**
- Complete artifact deployment guaranteed across all platforms
- Eliminated upload annotations with smart file existence checks
- Centralized version management through dedicated release creation job
- Professional packaging: DMG (macOS), NSIS/ZIP (Windows), DEB/TGZ (Linux)
- Complete developer guides for version management
- Updated development workflow documentation
- Comprehensive build and release procedures

**Technical**
- Memory management optimizations for SFML 3.0 integration
- Enhanced packaging reliability across all platforms
- All features from version 0.9.0 maintained

---

# What Changed Since Version 0.8.2

DeerPortal 0.9.0 "Welcome Back" included 111 commits of changes from version 0.8.2.

## SFML 3.0 Migration

**Complete Engine Upgrade**
- Upgraded from SFML 2.x to SFML 3.0.1
- Modern C++ implementation with smart pointer memory management
- Cross-platform compatibility verified for Windows, macOS, and Linux

## New Features

**Card Notification System**
- Overlay notifications when picking up cards
- Displays actual card texture matching picked card type
- Player portrait integration with context messages
- Click-to-dismiss functionality

**Intro Animation**
- Grid-based reveal effect using GLSL shaders
- 95x63 rectangle grid with zoom animation
- Deterministic random reveal pattern
- Skip control available

**Performance Improvements**
- VertexArray diamond rendering reduces draw calls by 99.1%
- SFML 3.0 triangle implementation
- Measured 20-80% FPS improvements across hardware configurations

## Platform Support

**macOS**
- Professional app bundle packaging
- Automated DMG installer creation

**Windows** 
- NSIS installer package
- Complete compilation guide
- Visual Studio integration

**Linux**
- TGZ and DEB package formats
- Minimal runtime dependencies

## Fixed Issues

Problems resolved from version 0.8.2:

- Asset loading path issues preventing sounds and textures from loading
- Memory management problems with SFML object initialization  
- Platform-specific library conflicts on macOS causing execution failures
- Cross-platform build system inconsistencies

## Game Features

- 0-4 player support with AI opponents
- Seasonal progression system
- Complete audio with sound effects and music
- 128 cards across 4 elements with strategic mechanics

**Download**: Available on GitHub with packages for all supported platforms.

## [0.9.2-pre.1] - Preparing Release & Windows Installer Fixes
- Ensured LICENSE file is included in Windows installer and all packaging (qmake, CMake).
- Updated version to 0.9.2-pre.1 for next release.
- Systematic review of installer/package file inclusions.