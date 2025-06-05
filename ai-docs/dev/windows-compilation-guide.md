# DeerPortal Windows Compilation Guide

## Overview

This guide provides comprehensive instructions for compiling DeerPortal on Windows using three different development environments:

1. **Visual Studio 2019/2022** (Recommended) - Full IDE experience with IntelliSense
2. **MSYS2** - Unix-like environment with modern package management
3. **Cygwin** - Traditional Unix compatibility layer

**Quick Decision Matrix:**
- **New to Windows development?** → Visual Studio 2019/2022
- **Cross-platform developer?** → MSYS2
- **Prefer Unix-like environment?** → Cygwin

## System Requirements

### Minimum Requirements
- **OS**: Windows 10 x64 (version 1903+) or Windows 11
- **RAM**: 4GB (8GB recommended for Visual Studio)
- **Storage**: 10GB free space (includes tools and dependencies)
- **CPU**: x64 processor with SSE2 support

### Required Software
- **Git for Windows** 2.30+
- **CMake** 3.16 or newer
- **SFML 3.0** (installation varies by method)
- **C++17 compatible compiler**

## Quick Start (Experienced Developers)

### Visual Studio 2022 (Fastest Setup)
```cmd
# Prerequisites: Visual Studio 2022 with C++ workload, vcpkg
git clone https://github.com/deerportal/deerportal.git
cd deerportal
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```

### MSYS2 (Unix-like)
```bash
# Prerequisites: MSYS2 with development packages
git clone https://github.com/deerportal/deerportal.git
cd deerportal
mkdir build && cd build
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

## Detailed Guides

### 📖 Step-by-Step Instructions
- **[Visual Studio Setup](windows-visual-studio-setup.md)** - Complete IDE setup with vcpkg
- **[MSYS2 Setup](windows-msys2-setup.md)** - Unix-like development environment
- **[Cygwin Setup](windows-cygwin-setup.md)** - Traditional Unix compatibility
- **[Dependencies Guide](windows-dependencies.md)** - SFML 3.0 installation methods

### 🔧 Support & Troubleshooting
- **[Common Issues](windows-troubleshooting.md)** - Solutions for frequent problems
- **[Performance Tips](windows-troubleshooting.md#performance)** - Optimization recommendations

## Build Types

### Release Build (Recommended for Gaming)
```cmd
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```
- Optimized for performance (-O3, LTO enabled)
- No debug symbols
- FPS counter hidden (unless explicitly enabled)

### Debug Build (Development)
```cmd
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```
- Debug symbols included
- No optimizations
- FPS counter always visible
- Better for debugging crashes

### FPS Counter Control
```cmd
# Enable FPS counter in Release builds
cmake -DCMAKE_BUILD_TYPE=Release -DSHOW_FPS_COUNTER=ON ..
```

## Asset Management

DeerPortal expects assets in the `assets/` directory relative to the executable:
```
DeerPortal.exe
assets/
├── img/
├── audio/
├── fonts/
└── data/
```

**CMake automatically handles asset deployment during build.**

## Packaging & Distribution

### NSIS Installer Creation
```cmd
# After successful build
cpack -G NSIS

# Generates: DeerPortal-0.9.0-Windows.exe
```

### Portable Distribution
```cmd
# Copy executable and assets
mkdir DeerPortal-Portable
copy Release\DeerPortal.exe DeerPortal-Portable\
xcopy assets DeerPortal-Portable\assets\ /E /I
```

## Advanced Configuration

### Static vs Dynamic Linking
```cmd
# Static linking (standalone executable)
cmake -DSFML_STATIC_LIBRARIES=ON ..

# Dynamic linking (requires SFML DLLs)
cmake -DSFML_STATIC_LIBRARIES=OFF ..
```

### Custom SFML Location
```cmd
cmake -DSFML_DIR="C:/SFML-3.0.0/lib/cmake/SFML" ..
```

## Project Structure After Build

```
deerportal/
├── build/
│   ├── Release/DeerPortal.exe     # Main executable
│   ├── assets/                    # Game assets (copied by CMake)
│   └── *.lib                      # Generated libraries
├── src/                           # Source code
├── assets/                        # Original assets
└── CMakeLists.txt                 # Build configuration
```

## Performance Expectations

### Build Times (Release Mode)
- **Visual Studio 2022**: ~3-5 minutes (first build)
- **MSYS2**: ~2-4 minutes
- **Cygwin**: ~4-6 minutes

### Runtime Performance
- **60+ FPS** on modern integrated graphics
- **~13MB DMG** equivalent size on Windows
- **Instant startup** with SSD storage

## Next Steps

1. **Choose your development environment** from the guides above
2. **Follow the specific setup instructions** for your chosen method
3. **Build and test** DeerPortal locally
4. **Read troubleshooting guide** if you encounter issues
5. **Consider contributing** to the project!

## Getting Help

- **GitHub Issues**: https://github.com/deerportal/deerportal/issues
- **Build Problems**: Check [troubleshooting guide](windows-troubleshooting.md)
- **SFML Issues**: Verify SFML 3.0 installation
- **CMake Problems**: Ensure CMake 3.16+ and proper PATH

---

**Last Updated**: January 2025 for DeerPortal 0.9.0 "Welcome Back"  
**SFML Version**: 3.0.x  
**CMake Version**: 3.16+  
**Windows Support**: Windows 10/11 x64 