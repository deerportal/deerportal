# DeerPortal AI Instructions & Context

## Project Overview
DeerPortal is a C++ game built with SFML 3.0.1. It's a cross-platform project supporting macOS, Linux, and Windows.

## Development Environment
- **Language:** C++17
- **Graphics Library:** SFML 3.0.1 (https://www.sfml-dev.org/development/changelog/)
- **Build System:** CMake (minimum version 3.16)
- **Repository Structure:** Modular architecture with separated functionalities

## Key Architecture Principles
- **Modular Design:** Code is split into separated files/classes for better maintainability
- **Reusable Components:** Components are designed to be reusable across different parts of the game
- **Cross-platform Compatibility:** Code must compile and run on macOS, Linux, and Windows

## Build & CI/CD Setup

### GitHub Actions Workflows
1. **cmake.yml** - Continuous Integration
   - Triggers on push/PR to master branch
   - Matrix build testing Linux, Windows, and macOS platforms (Intel + ARM)
   - Validates code compilation without creating release artifacts
   - Uses SFML 3.0.1 built from source for consistency
   - Architecture-specific builds for macOS (Intel x86_64 and Apple Silicon ARM64)

2. **release.yml** - Release Management  
   - Triggers on version tags (v*) and manual dispatch
   - Builds for macOS (DMG), Windows (NSIS installer), Linux (DEB/TGZ)
   - Creates GitHub releases with proper artifacts
   - Uses SFML 3.0.1 for all platforms

### Platform-Specific Notes

#### Linux (Ubuntu)
- Uses system dependencies via apt
- SFML built with shared libraries and system dependencies
- Static linking for final distribution

#### macOS
- SFML built from source with static libraries
- Creates .app bundles with proper framework bundling
- Targets macOS 10.15+ (Catalina)
- Uses Apple's native packaging (DMG)

#### Windows
- Uses MSVC toolchain via microsoft/setup-msbuild
- SFML built as static libraries to avoid DLL dependencies
- Creates NSIS installer packages
- Cached builds for faster CI

## File Organization
- `src/` - Main source code (modular game components)
- `assets/` - Game assets (textures, sounds, etc.)
- `packaging/` - Platform-specific packaging files
- `cmake_modules/` - Custom CMake modules
- `.data/` - Temporary data folder (gitignored)
- `ai-docs/` - AI-related documentation

## Git Strategy
- Main branch: `master`
- Version tags: `v*` format (e.g., v0.9.0)
- Comprehensive .gitignore covering all platforms and build artifacts
- Separate temporary data storage in `.data/` folder

## Version Management
- Version stored in `version.txt` file
- Automatically embedded in builds via CMake
- Used for packaging and release naming

## Dependencies Management
- SFML 3.0.1 built from source for consistency across platforms
- Cross-platform CMake configuration
- Platform-specific dependency handling in CI/CD

## Current Capabilities
- ✅ Cross-platform compilation (Linux, macOS, Windows)
- ✅ Automated CI testing on Linux, Windows, and macOS (Intel + ARM)
- ✅ Automated release builds for all platforms
- ✅ Package generation (DMG, NSIS, DEB, TGZ)
- ✅ Version management and artifact naming
- ✅ SFML 3.0.1 integration with proper static/shared library handling
- ✅ Architecture-specific macOS builds (Intel x86_64 and Apple Silicon ARM64)

## Development Guidelines
1. Always maintain cross-platform compatibility
2. Use modular design patterns
3. Update this file when making architectural changes
4. Test on multiple platforms before major releases
5. Keep dependencies minimal and well-documented
6. Use appropriate static/shared library linking per platform

## Known Working Configurations
- Ubuntu Latest + GCC + SFML 3.0.1 (shared libs)
- Windows Latest + MSVC + SFML 3.0.1 (static libs)  
- macOS Latest (ARM64) + Clang + SFML 3.0.1 (static libs)
- macOS 13 (Intel x86_64) + Clang + SFML 3.0.1 (static libs)

## Recent Changes
- 2024: Added Windows build testing to CI pipeline
- 2024: Added macOS build testing for both Intel and ARM architectures to CI pipeline
- 2024: Updated to SFML 3.0.1 across all workflows
- 2024: Implemented matrix build strategy for multi-platform CI testing
- 2024: Architecture-specific macOS builds with proper deployment targets
- 2025-01: Fixed GitHub Actions release packaging failures:
  - Updated macOS DMG creation to use custom `make dmg` instead of `cpack -G DragNDrop`
  - Fixed Linux packaging dependencies (removed SFML dev dependency for static builds)
  - Added fallback packaging methods for all platforms
  - Added comprehensive debugging to GitHub Actions workflows 