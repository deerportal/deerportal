# DeerPortal Release Notes

## Version 0.10.0-pre.1 "Lighting Effects Development" - July 26, 2025

🔧 **PRE-RELEASE - Foundation Development**

This pre-release establishes the technical foundation for implementing dynamic lighting effects during board initialization animation, featuring comprehensive planning and SFML 3.0.1 source validation.

### 📋 Foundation Work

**Technical Analysis & Planning**
- Comprehensive lighting system architecture analysis with three implementation approaches
- SFML 3.0.1 source code integration for technical validation and compatibility verification
- Performance requirements established: 60+ FPS with 112 light sources
- Memory optimization strategy: Single RenderTexture approach (~4MB overhead)

**Documentation System Enhancement**
- Complete final documentation covering particles, game states, sprites, animation systems, music, and sound
- Multi-plan technical analysis (GROK4, GEMINI25pro, OPUS4) with detailed comparison
- Optimal implementation selection: GEMINI25pro light map approach using native SFML blending

**Lighting System Design**
- Darkness overlay system for board initialization atmosphere
- Dynamic light sources attached to animated diamonds
- Native GPU blending using SFML's BlendAdd and BlendMultiply
- Scalable architecture with O(1) performance scaling

### 🎯 Next Steps
- Implement LightingManager class with light map rendering
- Create radial gradient light textures
- Integrate lighting effects with BoardInitializationAnimator
- Add configurable lighting parameters and debug visualization

## Version 0.9.9 "Animated Board Initialization" - July 25, 2025

🎯 **STABLE RELEASE - Major Visual Enhancement**

This stable release introduces a spectacular animated board initialization system that transforms the game startup experience with professional-quality visual effects.

### 🚀 Major Features

**Animated Board System**
- 112 diamonds spawn from four screen corners with smooth animation to final positions
- Bézier curve movement paths for natural, visually appealing motion
- Rotation and scaling effects during animation for dynamic visual impact
- Quadrant-based spawning with player-specific corner assignments
- Pixel-perfect positioning with seamless transition to static diamonds

**Performance & Quality**
- Single VertexArray draw call optimization for all 112 animated diamonds
- Cross-platform compatibility with Linux build fixes
- LLVM code formatting applied to entire codebase
- Debug system optimization with unified macro standards
- Professional A+ grade code quality implementation

**User Experience**
- Skip functionality via Space, Enter, or mouse click
- Staggered timing (0.05s delays) creates wave-like visual effect
- Multiple input methods for accessibility
- Graceful degradation with fallback to instant placement

### 🛠 Technical Improvements

- New `state_board_animation` integrated with existing game state management
- Proper math headers and M_PI definition for Linux compatibility
- Memory management improvements with RAII patterns
- Comprehensive error handling and debug system
- Extensive technical documentation in ai-docs/

### 📋 System Requirements

- SFML 3.0.1+
- CMake 3.16+
- C++17 compatible compiler
- Cross-platform: Linux, macOS, Windows

---

## Version 0.9.2 "Computer Player Card Notification Delay" - July 2025

🎮 **Final Release with Enhanced User Experience**

This release addresses a critical gameplay issue where computer players' card notifications appeared and disappeared too quickly for human players to read.

### 🚀 Major Improvements

- **4-Second Notification Delay**: Computer players now wait 4 seconds after picking up cards before proceeding to their next turn
- **Enhanced Game Flow**: Human players have sufficient time to read and understand card effects during computer turns
- **User Experience**: Fixed rapid computer turn progression that made following game state difficult
- **Timer System Integration**: Added `cardNotificationDelay` system seamlessly integrated with existing AI behavior

### 🔧 Technical Implementation

- **Files Modified**: `game.h`, `game.cpp`, `command.cpp`, `game-core.cpp`
- **New Timer System**: `CARD_NOTIFICATION_DELAY_TIME` constant set to 4.0 seconds
- **AI Behavior**: Computer players respect notification delay before making moves
- **Human Players**: Unaffected - continue normal gameplay flow

### 🎯 User Impact

- **Improved Readability**: Card notifications remain visible long enough to read
- **Better Game Understanding**: Players can follow computer actions more easily
- **Maintained Performance**: No impact on human player turn speed
- **Seamless Integration**: Delay feels natural within existing game rhythm

---

## Version 0.9.2-pre.2 "Windows Build Refinements" - January 2025

🔧 **Pre-release with Windows Build Improvements**

This pre-release focuses on enhancing Windows platform compatibility and build processes.

### 🚀 Major Improvements

**Windows Build Enhancements**
- Enhanced Windows executable generation and testing
- Resolved PowerShell execution policy compatibility issues
- Improved Windows build scripts for SFML 3.0.1 integration
- Version management updated for new pre-release cycle

**Known Issues**
- PowerShell execution policy may show error messages when running .exe files directly
- Application runs correctly despite PowerShell warnings
- Solutions documented in ai-docs/errors.md

---

## Version 0.9.1 "Stability Improvements" - July 2025

🔧 **Final Release with Major CI/CD Overhaul**

This maintenance release builds upon the solid foundation of version 0.9.0 "Welcome Back" with comprehensive CI/CD pipeline improvements, automated documentation generation, and enhanced cross-platform reliability.

### 🚀 Major Improvements

#### GitHub Actions CI/CD Pipeline Overhaul
- **Scout+Warrior Release Pattern**: Eliminated race conditions where multiple platform jobs competed to create releases
- **Smart Upload System**: Conditional file uploads prevent "no files matching glob pattern" annotations
- **Enhanced Cross-Platform Reliability**: Comprehensive debugging and error handling across all platforms
- **Professional Packaging**: DMG (macOS), NSIS/ZIP (Windows), DEB/TGZ (Linux) with consistent naming

#### Automated Documentation Generation
- **PDF Handbook Generation**: Automated conversion of HANDBOOK.md to professionally formatted PDF using Pandoc and LaTeX
- **Version-Specific Documentation**: Each release includes `DeerPortal-Handbook-VERSION.pdf` with complete game rules
- **Enhanced Release Notes**: Comprehensive installation instructions and system requirements in GitHub releases

#### Workflow Reliability Enhancements
- **Checkout Action Stabilization**: Added `fetch-depth: 0` preventing manifest parsing errors
- **File Pattern Fixes**: Flexible upload patterns handle varying CPack output naming conventions
- **Continue-on-Error Elimination**: Replaced error masking with smart file detection
- **Complete Artifact Deployment**: Guaranteed all platform artifacts upload to single release

### 🔧 Technical Improvements

#### Version System Overhaul
- **Single Source of Truth**: Reinforced `version.txt` as the authoritative version source
- **Automated Distribution**: Enhanced CMake-based version propagation to all components
- **Centralized Version Management**: All workflows now use single version source from create-release job
- **Cross-Platform Consistency**: Unified version handling across macOS, Windows, and Linux builds
- **Manual Update Documentation**: Clear guidelines for files requiring manual version updates

#### Development Workflow
- **Release Pipeline Improvements**: Better GitHub Actions integration with version management
- **Test Mode Enhancements**: Improved testing workflows without affecting production releases
- **Documentation Updates**: Comprehensive guides for version management and release processes

### 📋 What's Improved Since 0.9.0

#### Version Management
- Documented complete version management system with single source of truth
- Enhanced automation for version distribution across all build outputs
- Clear separation of automated vs. manual version update requirements

#### Build System
- Improved CMake version parsing and distribution mechanisms
- Enhanced package metadata consistency across all platforms
- Better error handling in automated build processes

#### Documentation
- Comprehensive version management documentation in `ai-instructions.md`
- Updated release process guidelines for maintainers
- Clear identification of all version-dependent files and update procedures

### 🎯 Maintained Features
All features from version 0.9.0 are preserved and enhanced:
- Complete SFML 3.0 compatibility
- Card Notification System with visual overlays
- Intro Shader Animation with grid-based reveal effect
- VertexArray Diamond Rendering optimization
- Cross-platform compatibility (macOS, Linux, Windows)
- Professional packaging for all platforms

This release focuses on improving the development experience and ensuring consistent, reliable releases while maintaining the excellent gameplay experience of DeerPortal.

---

## Version 0.9.0 "Welcome Back" - May 2025

🎉 **Welcome Back to the DeerPortal!** 🦌

After a successful SFML 3.0 migration, we're excited to present version 0.9.0 "Welcome Back" - a major milestone that brings DeerPortal into the modern era while maintaining the beloved gameplay experience.

**111 commits since 0.8.2** bring significant new features, performance improvements, and enhanced platform support across Windows, macOS, and Linux.

### 🚀 Major Features & Improvements

#### SFML 3.0 Migration Complete
- **Modern Graphics Engine**: Fully migrated to SFML 3.0 for improved performance and future-proofing
- **Enhanced Stability**: Modern C++ implementation with smart pointers for memory safety
- **Cross-Platform Excellence**: Seamless compatibility across macOS, Linux, and Windows

#### New Game Features Since 0.8.2

##### Card Notification System ✨ NEW
- **Visual Card Feedback**: Beautiful overlay notifications when players pick up cards
- **Card Image Display**: Shows actual card texture matching the picked card type
- **Player Portrait Integration**: Inline character sprites with "Player X" mentions
- **Context-Aware Messages**: Dynamic text describing card effects and targets
- **Content-First Overlay Sizing**: Smart layout system that adapts to content size
- **Gentle Animation**: Subtle blinking effect with click-to-dismiss functionality

##### Intro Shader Animation ✨ NEW  
- **Grid-Based Reveal Effect**: Professional GLSL shader with 95x63 rectangle grid
- **Dynamic Zoom Animation**: Each rectangle starts at 4x zoom, scales to normal size
- **Random Reveal Pattern**: Deterministic pseudo-random rectangle unveiling sequence
- **Asset Integration**: Uses existing `dp_intro_menu.png` for perfect visual consistency
- **Performance Optimized**: Direct-to-window rendering bypassing render texture overhead
- **Interactive Control**: Click anywhere to skip animation and proceed to menu

##### Enhanced Card System Documentation
- **Complete Card Reference**: Visual guide to all 128 cards across four elements
- **Strategic Documentation**: Detailed mechanics for Stop, Remove, Diamond, and Double Diamond cards
- **Element Targeting Rules**: Clear explanation of cross-element card effects
- **Advanced Strategy Guide**: Early/mid/late game card usage recommendations

#### Performance Optimizations

##### VertexArray Diamond Rendering (Issue #68) ⚡
- **Massive Draw Call Reduction**: From 112 individual sprites to 1 batched draw call (99.1% reduction)
- **SFML 3.0 Triangle Implementation**: Uses `sf::PrimitiveType::Triangles` for optimal performance
- **Content-First Positioning**: Fixed coordinate transformation for perfect tile alignment
- **Lazy Update System**: Only rebuilds vertex array when diamonds change state
- **Expected Performance Gains**: 20-80% FPS improvement depending on hardware

##### Advanced Shader Pipeline
- **Blur Effects**: Professional blur shader with configurable radius for atmospheric effects
- **Pixelation Effects**: Retro-style pixelation shader with threshold control
- **Dark Overlay Shaders**: Enhanced visual depth for menu and transition states
- **Performance Conditional Rendering**: Shaders only applied when beneficial

##### Memory Management Overhaul
- **Smart Pointer Migration**: Comprehensive conversion to `std::unique_ptr` for SFML objects
- **SFML 3.0 Constructor Compliance**: Proper texture binding for sprites and text objects
- **Memory Safety**: Eliminated potential memory leaks and improved stability
- **Resource Management**: Better asset loading and cleanup lifecycle

#### Technical Enhancements

##### Windows Development Support
- **Complete Compilation Guide**: Step-by-step Windows build instructions
- **Dependency Management**: Clear SFML 3.0 installation and linking procedures
- **Visual Studio Integration**: CMake configuration for Windows development environment
- **Cross-Platform Consistency**: Unified build system across Windows, macOS, and Linux

##### macOS Professional Packaging
- **Automated DMG Creation**: One-command DMG generation with professional installer UI
- **SFML Library Bundling**: Self-contained app bundles with isolated dependencies
- **Code Signing Integration**: Automatic ad-hoc signing for macOS security compliance
- **Library Conflict Resolution**: Eliminated "Class SFApplication is implemented in both" errors
- **App Bundle Optimization**: Proper macOS app structure with native icon and metadata

##### Build System Improvements
- **Enhanced CMake Configuration**: Better cross-platform dependency detection
- **Flexible SFML Linking**: Support for both static and shared SFML libraries
- **Debug/Release Optimization**: Conditional FPS counter and version display
- **Asset Path Management**: Robust asset loading across different build environments

### 🎮 Game Features Preserved
- **Classic Gameplay**: All beloved game mechanics remain unchanged
- **4-Player Strategy**: Complete support for 0-4 players with AI opponents
- **Seasonal Progression**: Dynamic weather and seasonal changes
- **Audio Excellence**: Full sound effects and background music support
- **Visual Polish**: All animations, shaders, and visual effects maintained

### 🔧 Developer Experience
- **Modular Architecture**: Improved code organization with separated functionalities
- **Comprehensive Documentation**: Updated AI documentation and memory banks
- **Build Reliability**: Zero compilation errors with modern toolchain support

### 📋 What's Fixed Since 0.8.2

#### Core SFML 3.0 Migration Issues
- Asset loading path issues that prevented sounds and textures from loading
- Memory management issues with SFML object initialization
- Event system compatibility with SFML 3.0's new variant-based events
- Rectangle API compatibility for collision detection and positioning
- Font and texture loading for all UI elements

#### Platform-Specific Fixes
- **macOS SFML Library Conflicts**: Eliminated "Class SFApplication is implemented in both" errors
- **Library Path Issues**: Fixed @rpath dependencies causing system library loading conflicts
- **macOS App Bundle Structure**: Resolved nested app bundle issues preventing proper execution
- **Windows Compilation Issues**: Resolved SFML 3.0 linking and dependency management on Windows
- **Cross-Platform Asset Loading**: Unified asset path resolution across all platforms

#### Performance and Rendering Fixes
- **Diamond Positioning Bug**: Fixed VertexArray coordinate transformation causing misaligned diamonds
- **Shader Compatibility**: Resolved GLSL version conflicts with macOS OpenGL implementation
- **Memory Leaks**: Eliminated potential memory leaks through smart pointer implementation
- **FPS Display Integration**: Proper conditional compilation for FPS counter display

#### Build System Fixes
- **CMake SFML Detection**: Improved SFML 3.0 library discovery across platforms
- **Dependency Isolation**: Resolved library conflicts in macOS app bundles
- **Build Configuration**: Enhanced Debug/Release build differentiation

### 🎯 Looking Forward
This "Welcome Back" release establishes a solid foundation for future enhancements while preserving the classic DeerPortal experience that players love. The modern codebase is ready for future features and improvements.

---

## Previous Releases

### Version 0.8.2 - January 2025
- **FPS Display Feature**: Real-time FPS counter with conditional compilation support
- **Window Icon Support**: Proper application icon display in window title bar
- **Performance Optimizations**: Enhanced rendering pipeline and resource management
- **SFML 3.0 Preparation**: Initial groundwork for SFML 3.0 migration
- **Build System Improvements**: Enhanced CMake configuration for cross-platform builds
- **Bug fixes and stability improvements**: Various crash fixes and memory optimizations

### Version 0.8.1 - 2024
- SFML 3.0 migration groundwork
- Code modernization
- Enhanced build system

### Version 0.7.3 "Pre-Snap" - 2018
- Snap package preparation
- Build system improvements

### Version 0.7.2 "Pre-Snap" - 2018  
- Snap package development
- Packaging refinements

### Version 0.7.1 "January Fix" - January 2018
- **Critical Fix**: Fixed January Eastern Egg problem (thanks Poul Sander)
- Resolved calendar-related crash issue
- Stability improvements for year-round gameplay

### Version 0.7.0 "Birthday" - June 2016
- **Notifications system**: New in-game messaging
- **Enhanced audio**: More sounds and background music
- **Smarter AI**: More aggressive and strategic computer opponents
- **Credits screen**: Proper attribution and acknowledgments
- **Test mode**: Developer testing functionality
- **Big diamond feature**: Special collectible gameplay element
- **Visual improvements**: Updated graphics and animations

### Version 0.6.0 "Beta" - June 2016
- **New dice graphics**: Beautiful dice artwork from Katia
- **Updated background**: Enhanced visual presentation
- **New end game screen**: Improved game completion experience
- **Bug fixes**: Multiple stability and gameplay issues resolved
- **Sorting improvements**: Better game state management
- **AI implementation**: Simple computer player functionality
- **New graphics**: Deer God artwork and rendered title screen

### Version 0.0.5 "Alpha" - May 2016
- **Portal mechanics**: Items removed when player reaches portal
- **Crash fixes**: Resolved game crash when taking too many items
- **Code cleanup**: General stability improvements
- **Debian packaging**: Better package management approach

### Version 0.0.4 "Alpha" - May 2016
- **Deer Mode**: Special transformation gameplay mechanic
- **Animated Characters**: Dynamic character movement and animation
- **Code cleanup**: Performance and stability improvements

### Version 0.0.3 "Alpha" - April 2016
- **Bubbles system**: Interactive UI elements and feedback
- **Code cleanup**: Architecture improvements

### Version 0.0.2 "Alpha" - April 2016
- **Core gameplay**: Fundamental game mechanics implemented
- **Basic UI**: Essential user interface elements

### Version 0.0.1 "Alpha" - April 2016
- **Initial release**: First public version
- **Basic game framework**: Core SFML implementation
- **Foundation**: Essential game loop and structure

---

## Development Timeline

**2025**: Version 0.9.0 "Welcome Back" - SFML 3.0 migration completed  
**2025**: Version 0.8.2 - Feature enhancements and optimizations  
**2018**: Version 0.7.x series - Snap packaging and final SFML 2.x versions  
**2016**: Version 0.6.x-0.7.0 - Major feature development period  
**2016**: Version 0.0.x series - Alpha development and core implementation  

**Developed with ❤️ by Rafal Zawadzki <bluszcz@gmail.com> using the [Devcarpet](https://devcarpet.net) platform.** 