# DeerPortal MSYS2 Setup Guide

## Overview

MSYS2 provides a Unix-like development environment on Windows with modern package management. It's ideal for developers familiar with Linux/macOS workflows who want consistency across platforms.

**Recommended for:**
- Cross-platform developers (Linux/macOS/Windows)
- Developers preferring command-line tools
- Teams using similar Unix-based workflows
- Contributors familiar with pacman package manager

## What is MSYS2?

MSYS2 is a software distribution and building platform for Windows that provides:
- **Bash shell** with Unix-like commands
- **pacman package manager** (from Arch Linux)
- **MinGW-w64 toolchain** for native Windows compilation
- **POSIX compatibility layer** for seamless development

## Installation Process

### 1. Download and Install MSYS2

#### Download MSYS2
```cmd
# Download from: https://www.msys2.org/
# Or use winget:
winget install MSYS2.MSYS2
```

#### Installation Steps
1. **Run installer** with default settings
2. **Install location**: `C:\msys64` (recommended)
3. **Launch MSYS2** after installation completes

#### Initial Setup
```bash
# Update package database and base packages
pacman -Syu

# Close terminal when prompted, then reopen MSYS2
# Complete the update
pacman -Su
```

### 2. Install Development Tools

#### Essential Build Tools
```bash
# Install base development tools
pacman -S base-devel
pacman -S mingw-w64-x86_64-toolchain

# Install Git
pacman -S git

# Install CMake
pacman -S mingw-w64-x86_64-cmake

# Install pkg-config (for dependency detection)
pacman -S mingw-w64-x86_64-pkg-config
```

#### Install SFML 3.0
```bash
# Install SFML development package
pacman -S mingw-w64-x86_64-sfml

# Verify installation
pacman -Qi mingw-w64-x86_64-sfml
```

### 3. Environment Configuration

#### Launch MinGW64 Environment
**Important**: Use **"MSYS2 MinGW x64"** terminal for compilation, not the default MSYS2 terminal.

#### Verify Installation
```bash
# Check compiler
gcc --version
g++ --version

# Check CMake
cmake --version

# Check SFML (should show version 3.x)
pkg-config --modversion sfml-all
```

## DeerPortal Build Process

### 1. Clone Repository

#### Using Git in MSYS2
```bash
# Navigate to desired directory
cd /c/Users/$USER/Documents  # or your preferred location

# Clone DeerPortal
git clone https://github.com/deerportal/deerportal.git
cd deerportal
```

### 2. Build Configuration

#### Create Build Directory
```bash
# Create and enter build directory
mkdir build && cd build

# Configure with CMake (Release build)
cmake -G "MSYS Makefiles" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/mingw64 \
      ..

# Alternative: Debug build
cmake -G "MSYS Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      ..
```

#### CMake Configuration Output
Look for successful messages:
```
-- The CXX compiler identification is GNU 13.2.0
-- SFML 3.0 found successfully
-- Configuring done
-- Generating done
-- Build files have been written to: /c/.../deerportal/build
```

### 3. Compilation

#### Build the Project
```bash
# Parallel build (adjust -j based on CPU cores)
make -j$(nproc)

# Or specific thread count
make -j4

# Verbose output (for debugging build issues)
make VERBOSE=1
```

#### Build Success Indicators
```bash
# Successful build should show:
[100%] Built target DeerPortal

# Check executable
ls -la DeerPortal.exe
file DeerPortal.exe
```

## Advanced MSYS2 Configuration

### 1. Package Management

#### Update System
```bash
# Regular updates
pacman -Syu

# Update specific packages
pacman -S mingw-w64-x86_64-sfml
```

#### Useful Development Packages
```bash
# Additional useful tools
pacman -S mingw-w64-x86_64-gdb          # Debugger
pacman -S mingw-w64-x86_64-ninja        # Alternative build system
pacman -S mingw-w64-x86_64-ccache       # Compiler cache
pacman -S mingw-w64-x86_64-clang        # Alternative compiler
```

### 2. Environment Optimization

#### Bash Configuration (~/.bashrc)
```bash
# Add to ~/.bashrc for convenience
export CC=gcc
export CXX=g++
export PKG_CONFIG_PATH="/mingw64/lib/pkgconfig"

# Aliases for development
alias ll='ls -la'
alias build='make -j$(nproc)'
alias rebuild='make clean && make -j$(nproc)'

# DeerPortal specific
alias dp-configure='cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..'
alias dp-debug='cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug ..'
```

#### Performance Tuning
```bash
# Enable compiler cache
export PATH="/mingw64/lib/ccache/bin:$PATH"

# Optimize for your CPU
export CXXFLAGS="-march=native -O3"
```

### 3. Development Workflow

#### Integrated Development
```bash
# Clean build
make clean

# Incremental build
make

# Install (if configured)
make install

# Run tests (if available)
make test
```

## Asset and Runtime Management

### 1. Asset Handling

#### Asset Directory Structure
MSYS2 follows Unix-like path conventions:
```bash
# DeerPortal expects assets relative to executable
./DeerPortal.exe
./assets/
├── img/
├── audio/
├── fonts/
└── data/
```

#### CMake Asset Copy
CMake automatically handles asset copying:
```cmake
# Handled by CMakeLists.txt
install(DIRECTORY assets DESTINATION "${ASSETS_BASE_PATH}/")
```

### 2. Running DeerPortal

#### From MSYS2 Terminal
```bash
# Direct execution
./DeerPortal.exe

# With parameters
./DeerPortal.exe --test

# Check dependencies
ldd DeerPortal.exe
```

#### From Windows
The compiled executable runs natively on Windows but may need MinGW64 DLLs in PATH.

## Debugging and Development Tools

### 1. GDB Debugging

#### Install and Setup
```bash
# Install GDB
pacman -S mingw-w64-x86_64-gdb

# Debug build
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Start debugging
gdb ./DeerPortal.exe
```

#### GDB Commands
```bash
# In GDB
(gdb) run                    # Start execution
(gdb) break main             # Set breakpoint
(gdb) continue               # Continue execution
(gdb) backtrace             # Show call stack
(gdb) list                  # Show source code
```

### 2. Code Analysis Tools

#### Static Analysis
```bash
# Install cppcheck
pacman -S mingw-w64-x86_64-cppcheck

# Analyze code
cppcheck --enable=all --std=c++17 src/
```

#### Memory Analysis
```bash
# Install Valgrind alternative for Windows
pacman -S mingw-w64-x86_64-drmemory

# Memory checking (limited Windows support)
drmemory -- ./DeerPortal.exe
```

## Distribution and Packaging

### 1. Standalone Executable

#### Check Dependencies
```bash
# List required DLLs
ldd DeerPortal.exe

# Typical output shows mingw64 dependencies:
# libgcc_s_seh-1.dll
# libstdc++-6.dll
# libsfml-*.dll
```

#### Bundle Dependencies
```bash
# Create distribution directory
mkdir DeerPortal-Distribution
cd DeerPortal-Distribution

# Copy executable
cp ../DeerPortal.exe .

# Copy required DLLs
cp /mingw64/bin/libgcc_s_seh-1.dll .
cp /mingw64/bin/libstdc++-6.dll .
cp /mingw64/bin/libwinpthread-1.dll .
cp /mingw64/bin/libsfml-*.dll .

# Copy assets
cp -r ../assets .
```

### 2. Portable Package Creation

#### Automated Script
```bash
#!/bin/bash
# create-package.sh

PROJECT_NAME="DeerPortal"
VERSION="0.9.0"
DIST_DIR="${PROJECT_NAME}-${VERSION}-MSYS2"

# Create distribution
mkdir -p "$DIST_DIR"

# Copy files
cp DeerPortal.exe "$DIST_DIR/"
cp -r assets "$DIST_DIR/"

# Copy MinGW dependencies
cp /mingw64/bin/{libgcc_s_seh-1.dll,libstdc++-6.dll,libwinpthread-1.dll} "$DIST_DIR/"
cp /mingw64/bin/libsfml-*.dll "$DIST_DIR/"

# Create archive
tar -czf "$DIST_DIR.tar.gz" "$DIST_DIR"

echo "Package created: $DIST_DIR.tar.gz"
```

## Integration with Other Tools

### 1. Code Editors

#### VS Code Integration
```bash
# Install VS Code from Windows
# Add MSYS2 terminal integration

# .vscode/settings.json
{
    "terminal.integrated.profiles.windows": {
        "MSYS2": {
            "path": "C:\\msys64\\usr\\bin\\bash.exe",
            "args": ["--login", "-i"],
            "env": {
                "MSYSTEM": "MINGW64",
                "CHERE_INVOKING": "1"
            }
        }
    },
    "terminal.integrated.defaultProfile.windows": "MSYS2"
}
```

#### Vim/Neovim
```bash
# Install neovim
pacman -S mingw-w64-x86_64-neovim

# Development setup with LSP
pacman -S mingw-w64-x86_64-clang-tools-extra  # for clangd
```

### 2. CMake GUI (Optional)
```bash
# Install CMake GUI
pacman -S mingw-w64-x86_64-cmake-gui

# Launch
cmake-gui
```

## Troubleshooting

### Common Issues

#### "Command not found"
```bash
# Check PATH
echo $PATH

# Should include /mingw64/bin
export PATH="/mingw64/bin:$PATH"
```

#### SFML Not Found
```bash
# Reinstall SFML
pacman -R mingw-w64-x86_64-sfml
pacman -S mingw-w64-x86_64-sfml

# Check installation
ls /mingw64/lib/libsfml*
```

#### Build Failures
```bash
# Clean rebuild
make clean
rm -rf CMakeFiles CMakeCache.txt
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

#### Missing DLL at Runtime
```bash
# Check dependencies
ldd DeerPortal.exe

# Add MinGW64 to Windows PATH or copy DLLs
export PATH="/mingw64/bin:$PATH"
```

### Performance Issues

#### Slow Compilation
```bash
# Use compiler cache
pacman -S mingw-w64-x86_64-ccache
export PATH="/mingw64/lib/ccache/bin:$PATH"

# Parallel builds
make -j$(nproc)
```

#### Runtime Performance
```bash
# Ensure Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Check compiler optimizations
make VERBOSE=1 | grep "\-O"
```

## Comparison with Other Methods

### MSYS2 vs Visual Studio
- **Pros**: Command-line workflow, package management, cross-platform consistency
- **Cons**: No integrated debugger GUI, steeper learning curve for Windows developers

### MSYS2 vs Cygwin
- **Pros**: Better Windows integration, modern package manager, active development
- **Cons**: Different from traditional Unix (less POSIX compliance than Cygwin)

## Next Steps

1. **Complete build** successfully
2. **Test DeerPortal** functionality
3. **Set up development** workflow with preferred editor
4. **Create distribution** package
5. **Explore advanced** MSYS2 features

---

**MSYS2 Version**: Latest (rolling release)  
**MinGW-w64**: GCC 13.2+  
**SFML Version**: 3.0.x  
**Package Manager**: pacman  
**Windows Integration**: Native executables 