# DeerPortal Windows Troubleshooting Guide

## Overview

This guide addresses common issues encountered when compiling and running DeerPortal on Windows across all three build environments: Visual Studio, MSYS2, and Cygwin.

## Quick Diagnostic Checklist

### Initial Environment Check
```cmd
# Check Windows version
winver

# Check system architecture
echo %PROCESSOR_ARCHITECTURE%
# Should be AMD64 for 64-bit

# Check available memory
systeminfo | findstr "Available Physical Memory"
```

### Compiler Verification
```bash
# For all environments, verify compiler
gcc --version || echo "GCC not found"
g++ --version || echo "G++ not found"
cmake --version || echo "CMake not found"
```

## Build System Issues

### CMake Configuration Problems

#### Issue: "CMake not found"
**Symptoms:**
```
'cmake' is not recognized as an internal or external command
```

**Solutions by Environment:**

**Visual Studio:**
```cmd
# Install CMake via Visual Studio Installer
# Or standalone: winget install Kitware.CMake
# Add to PATH: C:\Program Files\CMake\bin
```

**MSYS2:**
```bash
pacman -S mingw-w64-x86_64-cmake
# Ensure using MinGW64 terminal, not base MSYS2
```

**Cygwin:**
```bash
# Re-run setup-x86_64.exe and install cmake package
# Verify: which cmake
```

#### Issue: "Generator not found"
**Symptoms:**
```
CMake Error: Could not create named generator Visual Studio 17 2022
```

**Solutions:**
```cmd
# Check available generators
cmake --help

# Common fixes:
cmake -G "Visual Studio 16 2019" -A x64 ..    # VS 2019
cmake -G "Visual Studio 17 2022" -A x64 ..    # VS 2022
cmake -G "MSYS Makefiles" ..                  # MSYS2
cmake -G "Unix Makefiles" ..                  # Cygwin
```

#### Issue: "C++ compiler not found"
**Symptoms:**
```
No CMAKE_CXX_COMPILER could be found
```

**Solutions:**

**Visual Studio:**
```cmd
# Install "Desktop development with C++" workload
# Verify: cl.exe should be in PATH during VS Command Prompt
```

**MSYS2:**
```bash
# Install toolchain
pacman -S mingw-w64-x86_64-toolchain

# Verify
which g++
g++ --version
```

**Cygwin:**
```bash
# Install via setup-x86_64.exe:
# - gcc-core
# - gcc-g++
```

## SFML 3.0 Related Issues

### SFML Not Found

#### Issue: "Could not find SFML"
**Symptoms:**
```
CMake Error: Could not find package configuration file provided by "SFML"
```

**Solutions by Environment:**

**Visual Studio + vcpkg:**
```cmd
# Verify vcpkg installation
C:\vcpkg\vcpkg list sfml

# If empty, install:
C:\vcpkg\vcpkg install sfml:x64-windows

# Ensure CMake uses vcpkg:
cmake -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

**Visual Studio + Manual SFML:**
```cmd
# Download SFML 3.0 from sfml-dev.org
# Extract to C:\SFML-3.0.0
cmake -DSFML_DIR="C:/SFML-3.0.0/lib/cmake/SFML" ..
```

**MSYS2:**
```bash
# Install SFML package
pacman -S mingw-w64-x86_64-sfml

# If not available, try:
pacman -Sy  # Update package database
pacman -Ss sfml  # Search for SFML packages
```

**Cygwin:**
```bash
# SFML 3.0 usually not available in Cygwin repos
# Compile from source (see Cygwin setup guide)
# Or try: apt-cyg install libsfml-dev  # if apt-cyg is installed
```

#### Issue: "SFML version mismatch"
**Symptoms:**
```
SFML found version 2.5.1 but version 3.0 is required
```

**Solutions:**
```bash
# Remove old SFML versions first
# Visual Studio: vcpkg remove sfml
# MSYS2: pacman -R mingw-w64-x86_64-sfml
# Cygwin: Remove old installations manually

# Install SFML 3.0 specifically
```

### Runtime SFML Issues

#### Issue: "SFML DLLs not found"
**Symptoms:**
```
The program can't start because sfml-graphics-3.dll is missing
```

**Solutions:**

**Visual Studio + vcpkg:**
```cmd
# Copy DLLs to executable directory
copy "C:\vcpkg\installed\x64-windows\bin\sfml-*.dll" Release\

# Or add to PATH:
set PATH=C:\vcpkg\installed\x64-windows\bin;%PATH%
```

**MSYS2:**
```bash
# Copy required DLLs
cp /mingw64/bin/libsfml-*.dll .

# Or add to PATH:
export PATH="/mingw64/bin:$PATH"
```

**Cygwin:**
```bash
# Ensure SFML libraries are accessible
export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"
```

## Build Compilation Errors

### C++17 Standard Issues

#### Issue: "C++17 features not supported"
**Symptoms:**
```
error: 'std::unique_ptr' was not declared in this scope
```

**Solutions:**
```bash
# Verify compiler supports C++17
g++ -std=c++17 --version

# Add C++17 flag explicitly:
cmake -DCMAKE_CXX_STANDARD=17 ..

# Or set environment:
export CXXFLAGS="-std=c++17"
```

### Memory and Resource Issues

#### Issue: "Out of memory during compilation"
**Symptoms:**
```
g++: internal compiler error: Killed (program cc1plus)
virtual memory exhausted: Cannot allocate memory
```

**Solutions:**
```bash
# Reduce parallel jobs
make -j2  # Instead of make -j$(nproc)

# For Visual Studio:
# Close other applications
# Use x64 instead of x86 toolchain

# Increase virtual memory (Windows)
# System Properties → Advanced → Performance → Settings → Advanced → Virtual Memory
```

#### Issue: "Linker out of memory"
**Symptoms:**
```
collect2.exe: fatal error: ld terminated with signal 9 [Killed]
```

**Solutions:**
```bash
# Disable LTO temporarily
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG" ..

# Use gold linker (MSYS2/Cygwin)
export LDFLAGS="-fuse-ld=gold"
```

## Runtime Issues

### Asset Loading Problems

#### Issue: "Assets not found"
**Symptoms:**
```
Failed to load texture: assets/img/texture.png
```

**Solutions:**
```bash
# Check asset directory structure
ls -la assets/
# Should contain: img/, audio/, fonts/, data/

# Verify working directory
pwd
# Should be same directory as DeerPortal.exe

# For relative paths, ensure executable and assets are together:
DeerPortal.exe
assets/
├── img/
├── audio/
├── fonts/
└── data/
```

#### Issue: "Font loading errors"
**Symptoms:**
```
Failed to load font from file: assets/fonts/default.ttf
```

**Solutions:**
```bash
# Check font file permissions
ls -la assets/fonts/

# Windows: Ensure files aren't marked as "blocked"
# Right-click → Properties → Unblock

# Copy fonts to system fonts directory (alternative)
copy assets\fonts\*.ttf C:\Windows\Fonts\
```

### Performance Issues {#performance}

#### Issue: "Low FPS / Poor Performance"
**Symptoms:**
- FPS counter shows <30 FPS
- Sluggish gameplay
- High CPU usage

**Solutions:**

**Build Configuration:**
```bash
# Ensure Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Verify optimization flags
make VERBOSE=1 | grep -E "O[0-9]"
# Should show -O3 for Release builds
```

**Runtime Optimization:**
```bash
# Close unnecessary background programs
# Disable Windows Defender real-time protection for game directory
# Use dedicated graphics card (not integrated)

# Check graphics drivers are updated
# Windows: Device Manager → Display Adapters
```

**Environment-Specific:**
```bash
# MSYS2: Use native Windows paths for assets if possible
# Cygwin: Consider MSYS2 for better performance
# Visual Studio: Use Release configuration, not Debug
```

#### Issue: "Audio Problems"
**Symptoms:**
- No sound
- Audio crackling
- High audio latency

**Solutions:**
```bash
# Check audio drivers
# Windows: Settings → System → Sound → Troubleshoot

# Verify SFML audio module
ldd DeerPortal.exe | grep sfml-audio  # MSYS2/Cygwin
# Should show sfml-audio library

# Audio format issues - check asset files:
file assets/audio/*.ogg
# Should show valid Ogg Vorbis files
```

## Environment-Specific Issues

### Visual Studio Specific

#### Issue: "IntelliSense errors"
**Symptoms:**
- Red squiggly lines in code editor
- "Cannot open include file: 'SFML/Graphics.hpp'"

**Solutions:**
```cmd
# Regenerate CMake cache
# Delete .vs folder in project root
# Project → CMake → Delete Cache and Reconfigure

# Check CMakeSettings.json
# Ensure CMAKE_TOOLCHAIN_FILE points to vcpkg
```

#### Issue: "Debug symbols not loading"
**Symptoms:**
- Cannot set breakpoints
- Debugging shows assembly instead of source

**Solutions:**
```cmd
# Build Debug configuration
cmake --build . --config Debug

# Verify PDB files are generated
dir *.pdb

# Check debugging settings:
# Right-click project → Properties → Debugging
# Working Directory: $(OutDir)
```

### MSYS2 Specific

#### Issue: "Wrong terminal environment"
**Symptoms:**
```
bash: gcc: command not found
```

**Solutions:**
```bash
# Use correct MSYS2 terminal:
# NOT: MSYS2 MSYS
# USE: MSYS2 MinGW x64

# Verify environment:
echo $MSYSTEM
# Should output: MINGW64
```

#### Issue: "Package conflicts"
**Symptoms:**
```
error: failed to commit transaction (conflicting files)
```

**Solutions:**
```bash
# Update package database
pacman -Sy

# Force reinstall
pacman -S --overwrite '*' mingw-w64-x86_64-sfml

# Clean package cache
pacman -Scc
```

### Cygwin Specific

#### Issue: "Cygwin DLL version mismatch"
**Symptoms:**
```
Cygwin DLL version mismatch detected
```

**Solutions:**
```bash
# Update all Cygwin packages
# Re-run setup-x86_64.exe
# Select "Update" for all packages

# Alternatively, reinstall Cygwin completely
```

#### Issue: "POSIX path issues"
**Symptoms:**
```
Cannot find file: /cygdrive/c/path/to/file
```

**Solutions:**
```bash
# Use cygpath for path conversion
cygpath -w /cygdrive/c/path/to/file
# Output: C:\path\to\file

# Configure Git for Windows line endings
git config --global core.autocrlf input
```

## Distribution and Packaging Issues

### Missing Dependencies

#### Issue: "Application won't run on other machines"
**Symptoms:**
- Works on development machine
- Fails on clean Windows installation

**Solutions:**

**Identify Dependencies:**
```bash
# Visual Studio: Use Dependency Walker or dumpbin
dumpbin /dependents DeerPortal.exe

# MSYS2/Cygwin: Use ldd
ldd DeerPortal.exe
```

**Bundle Dependencies:**
```bash
# Visual Studio + vcpkg:
copy "C:\vcpkg\installed\x64-windows\bin\*.dll" Release\

# MSYS2:
cp /mingw64/bin/{libgcc_s_seh-1.dll,libstdc++-6.dll,libwinpthread-1.dll} .
cp /mingw64/bin/libsfml-*.dll .

# Cygwin:
cp /usr/bin/cygwin1.dll .
cp /usr/bin/cyg*.dll .  # All required Cygwin DLLs
```

### Installer Creation Issues

#### Issue: "NSIS installer fails"
**Symptoms:**
```
CPack Error: Cannot find NSIS compiler makensis
```

**Solutions:**
```cmd
# Install NSIS
winget install NSIS.NSIS

# Add to PATH
set PATH=C:\Program Files (x86)\NSIS;%PATH%

# Create installer
cpack -G NSIS -C Release
```

## Advanced Debugging Techniques

### Build System Debugging

#### CMake Verbose Output
```bash
# See actual compiler commands
make VERBOSE=1

# CMake debug mode
cmake --debug-output ..

# Show CMake variables
cmake -LAH ..
```

#### Preprocessor Debugging
```bash
# See preprocessor output
g++ -E -dM src/main.cpp > preprocessor_output.txt

# Check defined macros
grep -E "(SFML|DEERPORTAL)" preprocessor_output.txt
```

### Runtime Debugging

#### System Monitoring
```cmd
# Monitor file access
# Use Process Monitor (ProcMon) from Microsoft Sysinternals

# Check DLL loading
# Use Dependency Walker or Dependency Walker 2.2
```

#### Memory Analysis
```bash
# Windows: Use Application Verifier
# MSYS2: Install Dr. Memory
pacman -S mingw-w64-x86_64-drmemory
drmemory -- ./DeerPortal.exe

# Cygwin: Limited options, use valgrind alternatives
```

## Prevention and Best Practices

### Development Environment Setup

#### Version Consistency
```bash
# Pin SFML version in documentation
# Test on clean Windows installations
# Use consistent build flags across team

# Document exact tool versions
cmake --version > versions.txt
gcc --version >> versions.txt
```

#### Automated Testing
```bash
# Create test script
# test-build.bat for Windows batch
# test-build.sh for MSYS2/Cygwin

#!/bin/bash
# Basic build test
set -e
mkdir -p test-build
cd test-build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j2
./DeerPortal.exe --test
echo "Build test passed!"
```

### Documentation Updates

#### Keep Instructions Current
- Update compiler versions annually
- Verify SFML compatibility with new releases
- Test instructions on fresh Windows installations
- Update troubleshooting based on user feedback

## Getting Additional Help

### Information to Gather
When seeking help, provide:

```bash
# System information
systeminfo > system-info.txt

# Build environment
cmake --version
gcc --version
# Tool-specific version info

# Full error messages
# Copy complete error output, not just last line

# Build configuration
cat CMakeCache.txt | grep -E "(CMAKE_BUILD_TYPE|SFML|CMAKE_CXX_COMPILER)"
```

### Support Channels
- **GitHub Issues**: Include full system info and error logs
- **Community Forums**: SFML forum, Reddit r/cpp
- **Stack Overflow**: Tag with `sfml`, `cmake`, `windows`

---

**Last Updated**: January 2025  
**Covers**: Visual Studio 2019/2022, MSYS2, Cygwin  
**SFML Version**: 3.0.x  
**Windows Versions**: 10/11 x64 