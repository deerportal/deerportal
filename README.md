Devcarpet
=========
presents

Deerportal
----------

Deerportal is an open source (free as speech, free as beer) hybrid game that utilizes board and card game mechanisms. The world of the game is driven by four classical elements along with the Almighty Deer God.

**Current Version: 0.9.0 "Welcome Back" (May 2025)**

Homepage: https://devcarpet.net/deerportal/

Collect diamonds, execute actions on cards, and survive the chaos of nature! Compete against 3 other players.

Features
--------

* 0-4 players mode
* Changing seasons and dynamic weather
* Advanced AI opponents with different strategies  
* Open source (zlib / cc-by 4.0)
* Modern SFML 3.0 graphics engine
* Cross-platform support

**Platforms**: SFML 3.0, Linux, macOS, Windows.

## Installation

### Compilation

#### macOS

```bash
$ git clone https://github.com/bluszcz/deerportal-2025-Q3.git
$ cd deerportal-2025-Q3

# For a Release build (recommended for performance):
$ cmake -DCMAKE_BUILD_TYPE=Release .
# Or, for a Debug build (for development and debugging):
# $ cmake -DCMAKE_BUILD_TYPE=Debug .

# To always show the FPS counter (e.g., in a Release build for testing):
# $ cmake -DCMAKE_BUILD_TYPE=Release -DSHOW_FPS_COUNTER=ON .
# The FPS counter is automatically shown in Debug builds.

$ make

# Install to Applications (optional)
$ sudo make install

# Run the app directly
$ open DeerPortal.app

# Create macOS DMG package for distribution (recommended)
$ make dmg
```

**macOS Packaging Features:**
- Complete app bundle with proper icon and metadata
- Self-contained distribution with bundled SFML libraries
- Professional DMG installer with Applications symlink
- Code-signed for macOS security compatibility
- No external dependencies required for end users

#### Requirements
- **SFML 3.0** (automatically found by CMake if installed via Homebrew)
- **C++17 compatible compiler** (Clang++ 14+, GCC 9+, MSVC 2019+)
- **CMake 3.16** or newer

#### Homebrew Installation (macOS)
```bash
$ brew install sfml cmake
```

##### Debian 

If you would like to see Deer Portal in Debian repositories, there is an open bug: https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=826612 - please help :)

To make a deb package locally:

```
$ git clone https://github.com/bluszcz/deerportal-2025-Q3.git
$ cd deerportal-2025-Q3
$ fakeroot debian/rules binary
```

## What's New in 0.9.0 "Welcome Back"

- **SFML 3.0 Migration**: Complete upgrade to modern SFML 3.0 API
- **C++17 Support**: Modern C++ features and improved memory management
- **Enhanced Stability**: Smart pointer implementation for memory safety
- **Cross-Platform Excellence**: Improved macOS app bundle and Linux package support
- **Performance Optimizations**: Better resource management and loading
- **Professional macOS Packaging**: Native app bundle with proper icon, self-contained libraries, and DMG installer
- **Library Conflict Resolution**: Complete SFML library isolation for reliable macOS distribution

**Developed with [Devcarpet](https://devcarpet.net) platform.**
**Contact**: Rafal Zawadzki <bluszcz@gmail.com>
