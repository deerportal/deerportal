# DeerPortal Windows Dependencies Guide

## Overview

This guide focuses on installing and configuring SFML 3.0 for DeerPortal on Windows across different development environments. SFML (Simple and Fast Multimedia Library) is DeerPortal's primary dependency.

## SFML 3.0 Requirement

DeerPortal requires **SFML 3.0** specifically. Earlier versions (2.x) are not compatible due to API changes.

### Version Verification
```bash
# Check SFML version after installation
pkg-config --modversion sfml-all  # MSYS2/Cygwin
# Or check CMake output during configuration
```

## Installation Methods by Environment

### Visual Studio with vcpkg (Recommended)

#### Install vcpkg Package Manager
```cmd
# Install vcpkg globally
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio
.\vcpkg integrate install
```

#### Install SFML 3.0
```cmd
# Install SFML for 64-bit (recommended)
.\vcpkg install sfml:x64-windows

# Optional: Also install 32-bit
.\vcpkg install sfml:x86-windows

# Verify installation
.\vcpkg list sfml
```

#### CMake Configuration
```cmd
# Configure DeerPortal with vcpkg
cmake -G "Visual Studio 17 2022" -A x64 ^
      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
      ..
```

#### Benefits of vcpkg Method
- ✅ **Automatic dependency resolution**
- ✅ **No manual PATH configuration**
- ✅ **Easy updates**: `vcpkg upgrade sfml`
- ✅ **Consistent builds** across machines
- ✅ **Debug and Release** versions available

### Visual Studio with Manual Installation

#### Download SFML 3.0
1. Visit: https://www.sfml-dev.org/download.php
2. Download **SFML 3.0.0 - Visual C++ 15 (2017) - 64-bit**
3. Extract to `C:\SFML-3.0.0\`

#### Directory Structure
```
C:\SFML-3.0.0\
├── bin/          # Runtime DLLs
├── include/      # Header files
├── lib/          # Static/import libraries
└── examples/     # Sample code
```

#### CMake Configuration
```cmd
# Configure with manual SFML path
cmake -G "Visual Studio 17 2022" -A x64 ^
      -DSFML_DIR="C:/SFML-3.0.0/lib/cmake/SFML" ^
      ..
```

#### Runtime DLL Setup
```cmd
# Copy DLLs to output directory
copy "C:\SFML-3.0.0\bin\*.dll" Release\

# Or add to system PATH
set PATH=C:\SFML-3.0.0\bin;%PATH%
```

### MSYS2 Environment

#### Install SFML Package
```bash
# Update package database
pacman -Sy

# Install SFML 3.0
pacman -S mingw-w64-x86_64-sfml

# Verify installation
pacman -Qi mingw-w64-x86_64-sfml
ls /mingw64/lib/libsfml*
```

#### CMake Configuration
```bash
# SFML is automatically found in standard paths
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..

# If needed, specify explicitly:
cmake -G "MSYS Makefiles" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH=/mingw64 \
      ..
```

#### Runtime Libraries
```bash
# Check required DLLs
ldd DeerPortal.exe | grep sfml

# Copy to distribution if needed
cp /mingw64/bin/libsfml-*.dll .
```

### Cygwin Environment

Since SFML 3.0 is often not available in Cygwin repositories, we compile from source.

#### Install Build Dependencies
```bash
# Re-run setup-x86_64.exe and install:
# - gcc-core, gcc-g++
# - cmake, make
# - libfreetype-devel
# - libjpeg-devel
# - libpng-devel
# - libvorbis-devel
# - libflac-devel
# - libogg-devel
# - openal-devel
# - libX11-devel
# - libGL-devel
```

#### Compile SFML from Source
```bash
# Clone SFML 3.0
git clone --branch 3.0.x https://github.com/SFML/SFML.git
cd SFML

# Create build directory
mkdir build && cd build

# Configure SFML
cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DCMAKE_BUILD_TYPE=Release \
      -DSFML_BUILD_EXAMPLES=FALSE \
      -DSFML_BUILD_DOC=FALSE \
      ..

# Compile (takes 15-30 minutes)
make -j$(nproc)

# Install
make install

# Verify
ls -la /usr/local/lib/libsfml*
```

#### CMake Configuration for DeerPortal
```bash
# Configure DeerPortal with custom SFML
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DSFML_DIR=/usr/local/lib/cmake/SFML \
      ..
```

## Alternative Installation Methods

### Package Managers (Windows)

#### Chocolatey
```cmd
# Install Chocolatey first: https://chocolatey.org/
choco install sfml

# Note: May not have SFML 3.0 yet
```

#### Conan Package Manager
```cmd
# Install Conan: pip install conan
# Create conanfile.txt:
[requires]
sfml/3.0.0

[generators]
cmake

# Install dependencies
conan install . --build=missing
```

### Build from Source (All Environments)

#### Download Source Code
```bash
# Method 1: Git clone
git clone --branch 3.0.x https://github.com/SFML/SFML.git

# Method 2: Download archive
# https://github.com/SFML/SFML/releases/tag/3.0.0
```

#### Build Configuration Options
```bash
# Basic configuration
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DSFML_BUILD_EXAMPLES=OFF \
      -DSFML_BUILD_DOC=OFF \
      ..

# Advanced options
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=C:/SFML-3.0.0 \
      -DSFML_BUILD_EXAMPLES=OFF \
      -DSFML_BUILD_DOC=OFF \
      -DSFML_USE_STATIC_STD_LIBS=ON \
      ..
```

## Dependency Management

### Static vs Dynamic Linking

#### Dynamic Linking (Default)
```bash
# Pros: Smaller executable, shared libraries
# Cons: Must distribute DLLs with application

# CMake configuration (default)
cmake -DSFML_STATIC_LIBRARIES=OFF ..
```

#### Static Linking
```bash
# Pros: Self-contained executable
# Cons: Larger file size, potential licensing issues

# CMake configuration
cmake -DSFML_STATIC_LIBRARIES=ON ..
```

### SFML Modules

SFML consists of several modules:

#### Core Modules
- **sfml-system**: Base functionality (vectors, threads, etc.)
- **sfml-window**: Window and input management
- **sfml-graphics**: 2D graphics rendering
- **sfml-audio**: Sound and music playback
- **sfml-network**: Network communication

#### DeerPortal Usage
```cpp
// DeerPortal primarily uses:
#include <SFML/Graphics.hpp>  // Main graphics functionality
#include <SFML/Audio.hpp>     // Sound effects and music
#include <SFML/System.hpp>    // Utilities (Time, Vector2, etc.)
```

## Version Compatibility

### SFML 3.0 vs 2.x Differences

#### API Changes
```cpp
// SFML 2.x
sf::RenderWindow window(sf::VideoMode(800, 600), "Title");

// SFML 3.0 (similar, but internal changes)
sf::RenderWindow window(sf::VideoMode(800, 600), "Title");
```

#### Breaking Changes
- Internal texture management improvements
- OpenGL context handling changes
- Some deprecated functions removed

### Upgrading from SFML 2.x

#### Remove Old Versions
```bash
# Visual Studio + vcpkg
vcpkg remove sfml

# MSYS2
pacman -R mingw-w64-x86_64-sfml

# Manual installation
# Delete old SFML directories
```

## Environment-Specific Configuration

### Visual Studio Project Settings

#### Include Directories
```
# Automatically handled by CMake when using vcpkg or manual setup
# Manual setup: C:\SFML-3.0.0\include
```

#### Library Directories
```
# Automatically handled by CMake
# Manual setup: C:\SFML-3.0.0\lib
```

#### Linker Dependencies
```
# Debug builds:
sfml-graphics-d.lib
sfml-window-d.lib
sfml-system-d.lib
sfml-audio-d.lib

# Release builds:
sfml-graphics.lib
sfml-window.lib
sfml-system.lib
sfml-audio.lib
```

### MSYS2 Package Details

#### Package Information
```bash
# View package details
pacman -Qi mingw-w64-x86_64-sfml

# List package files
pacman -Ql mingw-w64-x86_64-sfml

# Check dependencies
pacman -Qd mingw-w64-x86_64-sfml
```

#### Update Process
```bash
# Update package database
pacman -Sy

# Upgrade SFML
pacman -S mingw-w64-x86_64-sfml

# Full system update
pacman -Syu
```

## Troubleshooting Dependencies

### Common Issues

#### "Could not find SFML"
```bash
# Check installation paths
ls /usr/local/lib/libsfml*     # Cygwin/source build
ls /mingw64/lib/libsfml*       # MSYS2
dir "C:\SFML-3.0.0\lib\*.lib"  # Manual Windows

# Verify CMake can find SFML
cmake --find-package -DNAME=SFML -DCOMPILER_ID=GNU -DLANGUAGE=CXX -DMODE=EXIST
```

#### Version Conflicts
```bash
# Multiple SFML versions installed
# Remove old versions completely before installing new

# Check which version CMake finds
cmake --find-package -DNAME=SFML -DCOMPILER_ID=GNU -DLANGUAGE=CXX -DMODE=COMPILE
```

#### Missing Dependencies
```bash
# SFML dependencies not found
# Install development packages for:
# - OpenGL (usually system-provided)
# - OpenAL (audio)
# - Freetype (font rendering)
# - JPEG, PNG libraries (image loading)
```

### Verification Steps

#### Test SFML Installation
```cpp
// test-sfml.cpp
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Test");
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();
        window.display();
    }
    
    std::cout << "SFML test successful!" << std::endl;
    return 0;
}
```

#### Compile Test
```bash
# MSYS2/Cygwin
g++ -std=c++17 test-sfml.cpp -lsfml-graphics -lsfml-window -lsfml-system -o test-sfml
./test-sfml.exe

# Visual Studio (use CMake or project files)
```

## Performance Considerations

### Debug vs Release Libraries

#### Debug Libraries
- Larger file sizes
- Additional runtime checks
- Better error messages
- Slower performance

#### Release Libraries
- Optimized for performance
- Smaller file sizes
- Minimal runtime checks
- Production-ready

### Static Linking Considerations

#### Advantages
- Self-contained executables
- No DLL dependency issues
- Easier distribution

#### Disadvantages
- Larger executable size
- Longer link times
- License implications (check SFML license)
- Memory usage (no shared libraries)

## Next Steps

1. **Choose installation method** based on your development environment
2. **Install SFML 3.0** using the appropriate method
3. **Verify installation** with test compilation
4. **Configure DeerPortal** build system
5. **Test DeerPortal** compilation and execution

---

**SFML Version Required**: 3.0.x  
**Supported Environments**: Visual Studio, MSYS2, Cygwin  
**Installation Methods**: vcpkg, manual, package manager, source compilation  
**Platform Support**: Windows 10/11 x64 