# DeerPortal Release Notes

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