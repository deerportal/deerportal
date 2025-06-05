# AI Instructions - DeerPortal Memory Bank

## Project Overview

DeerPortal is a sophisticated 2D game built with C++17 and SFML 3.0, featuring modular architecture and cross-platform support (macOS, Windows, Linux).

**Key Technologies:**
- **Language**: C++17 with modern standards
- **Graphics**: SFML 3.0 (Simple and Fast Multimedia Library)
- **Build System**: CMake 3.16+
- **Version**: 0.9.0 "Welcome Back"

## Development Standards

### Code Organization
- **Modular Design**: Split functionality into separate files/classes
- **Header Guards**: Use `#pragma once` consistently
- **Naming**: Clear, descriptive function and variable names
- **Comments**: Document complex algorithms and public interfaces

### Performance Focus
- **FPS Target**: 60+ FPS on modern hardware
- **Memory Management**: Smart pointers where appropriate
- **SFML Optimization**: Efficient texture and vertex buffer usage
- **Build Types**: Release builds with -O3 optimization

## Platform Support Status

### macOS (PRODUCTION READY ✅)
- **Build System**: Pure CMake solution with automatic packaging
- **Dependencies**: SFML 3.0 via Homebrew (`brew install sfml`)
- **Packaging**: Professional DMG creation (`make dmg`)
- **Distribution**: ~13MB self-contained DMG
- **Status**: All packaging issues resolved, no manual intervention needed

### Windows (COMPREHENSIVE DOCUMENTATION ✅)
- **Build Methods**: Visual Studio 2019/2022, MSYS2, Cygwin
- **Dependencies**: SFML 3.0 via vcpkg, package managers, or source compilation
- **Documentation**: Complete guides for all three environments
- **Packaging**: NSIS installer creation with CPack
- **Status**: Full documentation suite implemented for GitHub Issue #62

### Linux 
- **Build System**: Standard CMake with system package managers
- **Dependencies**: SFML 3.0 via apt/yum/pacman
- **Status**: Supported but needs documentation refinement

## Windows Compilation Documentation (NEW)

### Implementation Summary
Created comprehensive Windows compilation documentation addressing GitHub Issue #62:

#### Documentation Structure:
- **Main Guide**: `ai-docs/dev/windows-compilation-guide.md` - Central entry point
- **Visual Studio**: `ai-docs/dev/windows-visual-studio-setup.md` - IDE setup with vcpkg
- **MSYS2**: `ai-docs/dev/windows-msys2-setup.md` - Unix-like environment
- **Cygwin**: `ai-docs/dev/windows-cygwin-setup.md` - POSIX compatibility layer
- **Dependencies**: `ai-docs/dev/windows-dependencies.md` - SFML 3.0 installation
- **Troubleshooting**: `ai-docs/dev/windows-troubleshooting.md` - Common issues

#### Key Features:
- **Multi-Environment Support**: Three different Windows development approaches
- **SFML 3.0 Integration**: Detailed installation methods for each environment
- **Beginner-Friendly**: Step-by-step instructions with screenshots concepts
- **Troubleshooting**: Comprehensive problem-solving guide
- **Performance Optimization**: Build configuration and runtime tips
- **Distribution**: Packaging and deployment instructions

#### Target Audiences:
- **Visual Studio**: Windows developers new to DeerPortal, IDE preference
- **MSYS2**: Cross-platform developers, Unix-like workflow preference
- **Cygwin**: Developers requiring POSIX compliance, traditional Unix tools

## Architecture Knowledge

### Core Components

#### Game Engine (`src/game*.cpp/h`)
- **game.cpp/h**: Main game logic and state management
- **game-core.cpp/h**: Core game mechanics and rules
- **game-renderer.cpp/h**: Rendering pipeline and visual effects
- **game-input.cpp/h**: Input handling and user interaction
- **game-assets.cpp/h**: Asset loading and management

#### Entity System
- **character.cpp/h**: Player characters and NPCs
- **card*.cpp/h**: Card game mechanics
- **boardelem*.cpp/h**: Game board elements
- **particle*.cpp/h**: Particle effects system

#### UI Components
- **playerhud.cpp/h**: Player interface elements
- **guiwindow.cpp/h**: Window management system
- **hover.cpp/h**: Interactive hover effects
- **selector.cpp/h**: Selection mechanics

### Build System Understanding

#### CMake Configuration
- **C++17 Standard**: `set(CMAKE_CXX_STANDARD 17)`
- **SFML Integration**: `find_package(SFML 3 REQUIRED)`
- **Asset Management**: Automatic copying to build directory
- **Platform Detection**: Windows, macOS, Linux specific configurations

#### Asset Organization
```
assets/
├── img/          # Textures and sprites
├── audio/        # Sound effects and music
├── fonts/        # TrueType fonts
└── data/         # Game data files
```

## Recent Major Implementations

### Windows Documentation Suite (January 2025)
**Achievement**: Fully addressed GitHub Issue #62 with comprehensive Windows compilation guides

**Files Created**:
- `windows-compilation-guide.md` - Main entry point (4,500+ lines total)
- `windows-visual-studio-setup.md` - IDE setup with vcpkg integration
- `windows-msys2-setup.md` - Unix-like development environment
- `windows-cygwin-setup.md` - POSIX compatibility layer setup
- `windows-dependencies.md` - SFML 3.0 installation across environments
- `windows-troubleshooting.md` - Comprehensive issue resolution

**Key Innovations**:
- Multi-environment approach catering to different developer preferences
- Detailed SFML 3.0 installation methods (vcpkg, manual, source compilation)
- Environment-specific troubleshooting with actual error messages
- Performance optimization guides for each build method
- Distribution and packaging instructions

### macOS DMG Packaging (2024)
**Achievement**: Eliminated all SFML library conflicts and manual intervention

**Solution**: Pure CMake automation with:
- Automatic library bundling to `Frameworks/`
- Complete @rpath dependency resolution
- Professional DMG creation with background and Applications symlink
- Ad-hoc code signing for macOS compatibility

## Development Workflow

### Git Integration
- **Branches**: Feature-based development with clear naming
- **Commits**: Descriptive messages explaining changes
- **Issues**: GitHub issue tracking for bugs and enhancements
- **Pull Requests**: Code review process for major changes

### Testing Strategy
- **Build Verification**: Test on all supported platforms
- **Asset Validation**: Ensure all resources load correctly
- **Performance Testing**: Monitor FPS and memory usage
- **Distribution Testing**: Verify packaged applications work on clean systems

## Documentation Standards

### Memory Bank Updates
Always update `ai-instructions.md` with:
- **New Features**: Document implementation approach and key decisions
- **Architecture Changes**: Update component relationships
- **Build System**: Note CMake configuration changes
- **Platform Support**: Update status and known issues

### Modular Documentation
- **Separation of Concerns**: Each guide focuses on specific environment/topic
- **Cross-References**: Link related documents appropriately
- **Practical Examples**: Include actual command-line examples
- **Troubleshooting**: Document real issues and tested solutions

## Performance Considerations

### SFML Optimization
- **Texture Management**: Minimize texture binds, use texture atlases
- **Vertex Arrays**: Batch rendering operations
- **Frame Rate**: Target 60 FPS, implement frame limiting
- **Memory**: Monitor for leaks, especially in particle systems

### Build Optimization
- **Release Builds**: Use `-O3` optimization, enable LTO
- **Debug Builds**: Include debug symbols, disable optimizations
- **Asset Optimization**: Compress textures and audio appropriately

## Known Issues and Solutions

### Windows-Specific
- **SFML Version**: Must use 3.0+, incompatible with 2.x series
- **DLL Dependencies**: Document required runtime libraries
- **Path Issues**: Handle Windows vs Unix path separators

### Cross-Platform Considerations
- **Line Endings**: Configure Git for appropriate line ending handling
- **File Paths**: Use CMake variables for asset paths
- **Compiler Differences**: Test with GCC, Clang, and MSVC

## Future Priorities

### Documentation
1. **Linux Compilation Guide** - Similar comprehensive approach as Windows
2. **Contributor Guide** - Development environment setup
3. **API Documentation** - Generate from code comments

### Technical Improvements
1. **Test Suite** - Automated testing framework
2. **CI/CD Pipeline** - Automated builds and testing
3. **Performance Profiling** - Identify optimization opportunities

## AI Assistant Guidelines

### Code Changes
- **Always modular**: Split large functions into smaller, focused ones
- **Header organization**: Keep interface clean, implementation private
- **Error handling**: Check return values, use exceptions appropriately
- **Memory safety**: Prefer smart pointers, avoid raw new/delete

### Documentation Updates
- **Update this file** after major implementations
- **Cross-reference** related documentation
- **Include examples** that actually work
- **Test instructions** on clean systems when possible

### Build System
- **CMake best practices**: Use modern CMake features
- **Platform detection**: Handle Windows, macOS, Linux differences
- **Dependency management**: Clear instructions for each platform
- **Asset handling**: Ensure assets are available at runtime

---

**Last Updated**: January 2025 - Windows Compilation Documentation Implementation  
**Next Priority**: Test Windows documentation on clean systems, address any gaps  
**Maintenance**: Update SFML compatibility as new versions release