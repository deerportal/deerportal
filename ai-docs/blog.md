# DeerPortal Version 0.9.2-pre.2 - Windows Build Refinements (PRE-RELEASE)

**Development Date**: January 2025  
**Platforms**: Windows (Primary), macOS, Linux

## Version 0.9.2-pre.2 Progress

**Windows Build Improvements**
- Enhanced Windows executable generation and testing
- Resolved PowerShell execution policy compatibility issues
- Improved Windows build scripts for SFML 3.0.1 integration
- Version management updated for new pre-release cycle
- ✅ Grid reveal intro shader loading successfully verified and fixed

**Fullscreen Implementation Planning**
- Created comprehensive plan for cross-platform fullscreen toggle
- F key chosen as toggle (F11 problematic on macOS)
- Detailed architecture for SFML 3.0.1 window recreation approach
- Platform-specific considerations documented
- Resource management and error handling strategies defined

**Known Issues**
- PowerShell execution policy may show error messages when running .exe files directly
- Application runs correctly despite PowerShell warnings
- Solutions documented in ai-docs/errors.md

**Development Notes**
- Systematic approach to Windows platform compatibility
- Grid reveal intro shader loading successfully verified
- Continuous integration improvements for Windows builds
- Fullscreen feature planned for future implementation

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