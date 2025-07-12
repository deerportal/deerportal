# DeerPortal Version 0.9.1 - Stability Improvements

**Release Date**: July 2025  
**Platforms**: Windows, macOS, Linux

## Version 0.9.1 Updates

This maintenance release focuses on development infrastructure improvements and build system reliability.

**Version Management**
- Complete version management system documentation
- Streamlined release process with automated version distribution  
- Enhanced CMake configuration for consistent version tracking

**Build System**
- Improved cross-platform build processes
- Enhanced error handling in packaging systems
- Better asset loading stability across Windows, macOS, and Linux

**Documentation**
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