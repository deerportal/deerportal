# DeerPortal Release Notes

## Version 0.9.0 "Welcome Back" - May 2025

🎉 **Welcome Back to the DeerPortal!** 🦌

After a successful SFML 3.0 migration, we're excited to present version 0.9.0 "Welcome Back" - a major milestone that brings DeerPortal into the modern era while maintaining the beloved gameplay experience.

### 🚀 Major Features & Improvements

#### SFML 3.0 Migration Complete
- **Modern Graphics Engine**: Fully migrated to SFML 3.0 for improved performance and future-proofing
- **Enhanced Stability**: Modern C++ implementation with smart pointers for memory safety
- **Cross-Platform Excellence**: Seamless compatibility across macOS, Linux, and Windows

#### Technical Enhancements
- **Modern C++17**: Upgraded from C++11 to C++17 for better performance and modern language features
- **Smart Memory Management**: Comprehensive conversion to `std::unique_ptr` for SFML objects
- **Improved Asset Loading**: Fixed asset path issues for reliable resource loading across all platforms
- **Enhanced Build System**: Updated CMake configuration for modern dependency management

#### Platform Improvements
- **macOS App Bundle**: Perfect integration with macOS app bundle structure
- **Linux Packaging**: Updated Debian packages and Snap support
- **Windows Compatibility**: Maintained full Windows support with improved build process

#### macOS Packaging Excellence
- **Professional DMG Creation**: Automated DMG generation with proper installer UI
- **Library Conflict Resolution**: Complete SFML library isolation preventing system conflicts
- **Self-Contained Distribution**: All dependencies bundled - no external installations required
- **Code-Signed App Bundles**: Automatic ad-hoc signing for macOS security compatibility
- **Native Icon Integration**: Custom DeerPortal icon properly displayed in Dock and Finder
- **Pure CMake Solution**: Zero manual intervention - fully automated packaging pipeline

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

### 📋 What's Fixed
- Asset loading path issues that prevented sounds and textures from loading
- Memory management issues with SFML object initialization
- Event system compatibility with SFML 3.0's new variant-based events
- Rectangle API compatibility for collision detection and positioning
- Font and texture loading for all UI elements
- **macOS SFML Library Conflicts**: Eliminated "Class SFApplication is implemented in both" errors
- **Library Path Issues**: Fixed @rpath dependencies causing system library loading conflicts
- **macOS App Bundle Structure**: Resolved nested app bundle issues preventing proper execution

### 🎯 Looking Forward
This "Welcome Back" release establishes a solid foundation for future enhancements while preserving the classic DeerPortal experience that players love. The modern codebase is ready for future features and improvements.

---

## Previous Releases

### Version 0.8.2 - January 2025
- FPS display feature
- Window icon support  
- Performance optimizations
- Bug fixes and stability improvements

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