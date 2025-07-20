# DeerPortal Development Blog

## July 19, 2025 - Spectacular Particle Effects (v0.9.7-pre.2)

Made all particle effects dramatically more visible and spectacular. Enhanced all parameters for maximum visual impact:

**Particle Count Doubles:** DIAMOND_BURST (6→12), CARD_COLLECT (4→8), STOP_CARD (8→16), diamond x2 (10→20)

**Speed Increases:** DIAMOND_BURST (120→180), CARD_COLLECT (80→140), STOP_CARD (60→100), diamond x2 (140→220)

**Scale Increases:** DIAMOND_BURST (0.5→0.8), CARD_COLLECT (0.3→0.6), STOP_CARD (0.4→0.7), diamond x2 (0.8→1.0)

**Lifetime Extensions:** DIAMOND_BURST (1.2→1.8s), CARD_COLLECT (0.8→1.4s), STOP_CARD (1.0→1.6s), diamond x2 (1.8→2.2s)

Removed unused hardcoded radius limitation. All collection effects now provide highly visible, long-lasting spectacular visual feedback.

## July 19, 2025 - Linux Distribution Fixes (v0.9.6)

Fixed critical Linux distribution issues affecting AppImage and tar.gz packages. Updated asset path resolution in filetools.h to properly locate assets in `/usr/share/games/deerportal/` for both AppImage and standard Linux installations. Modified CI/CD workflows (cmake.yml, test-release.yml, release.yml) to install assets in correct location. Added Linux-specific binary detection logic to resolve assets relative to executable location.

## July 19, 2025 - Version 0.9.6 Release - Performance and Architecture

Version 0.9.6 stable release completed with comprehensive documentation updates and architecture improvements. Updated all version files from 0.9.6-pre.2 to stable 0.9.6 across version.txt, debian/changelog, snapcraft.yaml, and appdata.xml. Enhanced modular system architecture documentation and synchronized project files.

## July 19, 2025 - Documentation Synchronization (v0.9.6-pre.2)

Updated project documentation to reflect current codebase state. Key changes include removal of legacy particle system files, addition of new utility systems (window-manager, safe-asset-loader, error-handler), and updated file structure documentation. The modular architecture improvements have been properly documented with current file counts and system organization.

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