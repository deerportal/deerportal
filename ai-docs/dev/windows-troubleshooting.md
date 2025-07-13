# Windows Troubleshooting Guide

## Visual Studio Installation Issues

### Problem: CMake Cannot Find Visual Studio Generator
**Symptoms:**
- `CMake Error: Generator "Visual Studio 17 2022" could not find any instance of Visual Studio`
- CMake picks up older VS 2019 BuildTools instead of VS 2022

**Root Cause:**
- Multiple Visual Studio versions installed
- VS 2022 not properly registered with CMake
- Environment variables pointing to wrong VS version

**Solution:**
1. **Clean Installation Approach:**
   - Uninstall all Visual Studio versions
   - Reinstall Visual Studio 2022 Community with required components:
     - Desktop development with C++
     - C++ CMake tools for Linux
     - MSVC v143 compiler toolset
     - Windows 10/11 SDK (latest)

2. **Use Developer Command Prompt:**
   ```cmd
   # Always use "Developer Command Prompt for VS 2022"
   # Not regular PowerShell or Command Prompt
   ```

3. **Let CMake Auto-detect Generator:**
   ```cmd
   # Don't specify -G generator, let CMake choose
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```

## SFML 3.0.1 Build Issues

### Problem: SFML 3.0.1 Not Available via Package Managers
**Symptoms:**
- vcpkg only has SFML 2.5.1
- Chocolatey doesn't have SFML 3.0.1
- Pre-built binaries hard to find

**Solutions:**
1. **Build from Source (CI/CD Method):**
   ```cmd
   git clone --depth 1 --branch 3.0.1 https://github.com/SFML/SFML.git sfml-source
   cd sfml-source
   cmake -B build -DCMAKE_BUILD_TYPE=Release -DSFML_BUILD_EXAMPLES=OFF -DSFML_BUILD_DOC=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="C:/SFML"
   cmake --build build --config Release
   cmake --install build
   ```

2. **Use CI/CD Artifacts:**
   ```cmd
   # Download pre-built binaries from SFML CI/CD
   Invoke-WebRequest -Uri "https://artifacts.sfml-dev.org/by-branch/master/windows-vc17-64.zip" -OutFile "sfml-windows.zip"
   Expand-Archive -Path "sfml-windows.zip" -DestinationPath "SFML-3.0.1"
   ```

## Asset Loading Issues in Windows Build

### Problem: Assets Not Found in Installed Version
**Symptoms:**
- `Failed to open sound file (couldn't open stream)`
- `Provided path: assets/audio/dice.ogg`
- `Absolute path: C:\Program Files\DeerPortal 0.9.1\assets\audio\dice.ogg`

**Root Cause:**
- Assets not properly copied during installation
- CMake install rules incomplete
- qmake DISTFILES not working correctly

**Investigation Status:**
- Assets exist in source tree
- Build scripts declare asset copying
- Issue occurs in final installed package

**Next Steps:**
- Test local build with assets
- Verify CMake install rules
- Check NSIS packaging script

## Build Environment Setup

### Recommended Approach (CI/CD Style)
1. **Use Fresh VS 2022 Installation**
2. **Build SFML from Source**
3. **Use Auto-detected CMake Generator**
4. **Build in Developer Command Prompt**

### Commands (Exact CI/CD Sequence)
```cmd
# 1. Build SFML
git clone --depth 1 --branch 3.0.1 https://github.com/SFML/SFML.git sfml-source
cd sfml-source
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSFML_BUILD_EXAMPLES=OFF -DSFML_BUILD_DOC=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="C:/SFML"
cmake --build build --config Release
cmake --install build
cd ..

# 2. Build DeerPortal
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/SFML" -DSFML_DIR="C:/SFML/lib/cmake/SFML" -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build build --config Release --verbose

# 3. Package
cd build
make package
```

## Successful Build Confirmation (0.9.2-pre.1)

### **✅ SUCCESS: Windows Build Working**
**Date:** 2025-07-13  
**Environment:** Windows 10.0.19045, Visual Studio 2022 Community (17.14.8), SFML 3.0.1

**Build Results:**
- ✅ **DeerPortal.exe** successfully compiled (2.8MB)
- ✅ **SFML 3.0.1** built from source and linked correctly
- ✅ **All game systems working:** Audio, Graphics, Shaders
- ✅ **No compilation errors** (only harmless GCC flag warnings)
- ✅ **Game launches and runs** with message: "Grid reveal intro shader loaded successfully with intro texture"

**Build Command Sequence:**
```cmd
# This exact sequence works:
git clone --depth 1 --branch 3.0.1 https://github.com/SFML/SFML.git sfml-source
cd sfml-source
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSFML_BUILD_EXAMPLES=OFF -DSFML_BUILD_DOC=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="C:/SFML"
cmake --build build --config Release
cmake --install build
cd ..
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/SFML" -DSFML_DIR="C:/SFML/lib/cmake/SFML" -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build build --config Release --verbose
```

### **🎉 MAJOR BREAKTHROUGH: Asset Packaging FIXED (0.9.2-pre.1)**
**Date:** 2025-07-13  
**Status:** ✅ **COMPLETE SUCCESS**

**Problem SOLVED:** Assets are now automatically copied to build directory during compilation.

**Solution Implemented:**
```cmake
# Windows: Copy assets to build directory for immediate use
add_custom_command(TARGET DeerPortal POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:DeerPortal>/assets
  COMMENT "Copying assets to Windows build directory"
)
```

**Verification Results:**
- ✅ **POST_BUILD command executes successfully** during build
- ✅ **All assets copied to build/Release/assets/** automatically
- ✅ **Critical dice.ogg file present** and accessible
- ✅ **Game runs without manual asset copying**
- ✅ **CMake install process works perfectly**
- ✅ **LICENSE file properly included**

**Installation Test Results:**
```cmd
# Test installation works perfectly
cmake --install . --prefix test_install
cd test_install
.\DeerPortal.exe  # Runs successfully!
```

**Files Successfully Installed:**
- ✅ DeerPortal.exe (2.8MB)
- ✅ LICENSE file (searchable)
- ✅ Complete assets directory structure
- ✅ All audio files (including dice.ogg)
- ✅ All image assets and shaders
- ✅ Runtime libraries

### **⚠️ RESOLVED: Asset Packaging Issue**
**Previous Problem:** Assets were not automatically copied to build directory during compilation.
**Previous Symptom:** Game failed with "Failed to open sound file" when run from build directory without manual asset copying.
**Previous Workaround:** Manual copy: `Copy-Item -Recurse -Force assets build\Release\`
**Current Status:** ✅ **FIXED** - Assets automatically copied during build process using POST_BUILD commands.

### **🔧 Enhanced qmake Configuration**
**qmake asset handling also enhanced:**
```qmake
# Asset installation configuration
assets.path    = $${DESTDIR}/assets/
assets.files   = assets/*
INSTALLS        += assets

# Add PRE_TARGETDEPS for build-time copying (CI ENHANCEMENT)
win32 {
  assets_copy.commands = $(COPY_DIR) $$shell_path($$PWD/assets) $$shell_path($$OUT_PWD/assets)
  assets_copy.depends = FORCE
  QMAKE_EXTRA_TARGETS += assets_copy
  PRE_TARGETDEPS += assets_copy
}

# LICENSE file inclusion
license.path = $${DESTDIR}/
license.files = LICENSE
INSTALLS += license
```

## Known Issues
- PowerShell may have issues with complex command chains
- Use cmd.exe or Developer Command Prompt for reliability
- Multiple VS versions cause CMake confusion
- ✅ **Asset packaging RESOLVED in 0.9.2-pre.1**
- ✅ **NSIS installer license display RESOLVED in 0.9.2-pre.1**
- ✅ **Start Menu integration RESOLVED in 0.9.2-pre.1**

## NSIS Installer Fixes (0.9.2-pre.1)
**License Display Issue**: Fixed by adding `CPACK_RESOURCE_FILE_LICENSE` configuration
**Start Menu Integration**: Added proper shortcut creation with `CPACK_NSIS_CREATE_ICONS_EXTRA`
**Search Visibility**: DeerPortal now appears in Windows Start Menu search after installation 