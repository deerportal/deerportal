# DeerPortal Cygwin Setup Guide

## Overview

Cygwin provides a POSIX-compatible Unix environment on Windows, offering maximum compatibility with traditional Unix development workflows. It's ideal for developers who need full POSIX compliance or are migrating from Unix systems.

**Recommended for:**
- Developers requiring POSIX compliance
- Migration from traditional Unix systems
- Teams using complex Unix-specific build scripts
- Developers familiar with traditional Unix toolchains

## What is Cygwin?

Cygwin is a POSIX compatibility layer that provides:
- **Full POSIX API implementation** via cygwin1.dll
- **Unix-like environment** with bash, make, gcc
- **X11 support** for graphical Unix applications
- **Traditional Unix toolchain** familiar to Unix developers

**Note**: Unlike MSYS2, Cygwin executables depend on cygwin1.dll and run in the Cygwin environment.

## Installation Process

### 1. Download and Install Cygwin

#### Download Cygwin Installer
```cmd
# Download from: https://www.cygwin.com/
# Select architecture:
# - setup-x86_64.exe (64-bit, recommended)
# - setup-x86.exe (32-bit, legacy)
```

#### Installation Steps
1. **Run setup-x86_64.exe** as Administrator
2. **Choose Install from Internet**
3. **Root Directory**: `C:\cygwin64` (default)
4. **Local Package Directory**: `C:\cygwin64\downloads`
5. **Internet Connection**: Direct Connection
6. **Download Site**: Select a mirror (choose geographically close)

### 2. Package Selection

#### Essential Base Packages
Select these packages during installation:

**Development Tools:**
- `gcc-core` - GNU Compiler Collection (C)
- `gcc-g++` - GNU Compiler Collection (C++)
- `make` - GNU Make
- `cmake` - Cross-platform build system
- `git` - Distributed version control
- `pkg-config` - Package configuration tool

**Build Dependencies:**
- `autoconf` - Automatic configure script builder
- `automake` - Tool for generating Makefile.in
- `libtool` - Generic library support script
- `binutils` - Binary utilities

**System Libraries:**
- `libX11-devel` - X11 development files
- `libGL-devel` - OpenGL development files
- `libopenal-devel` - OpenAL development files

#### Search and Add Packages
1. **View**: Select "Full" to see all packages
2. **Search**: Type package name in search box
3. **Select**: Click "Skip" to change to version number
4. **Dependencies**: Let Cygwin resolve automatically

### 3. SFML 3.0 Installation

Since Cygwin may not have SFML 3.0 in repositories, we'll compile from source:

#### Prerequisites for SFML Compilation
```bash
# Install additional dependencies
# Re-run setup-x86_64.exe and add:
# - libfreetype-devel
# - libjpeg-devel  
# - libpng-devel
# - libvorbis-devel
# - libflac-devel
# - libogg-devel
# - openal-devel
```

#### Compile SFML 3.0 from Source
```bash
# Launch Cygwin Terminal
# Navigate to home directory
cd ~

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

# Compile SFML (this may take 15-30 minutes)
make -j$(nproc)

# Install SFML
make install

# Verify installation
ls -la /usr/local/lib/libsfml*
```

## DeerPortal Build Process

### 1. Environment Setup

#### Launch Cygwin Terminal
Use the Cygwin Terminal (not Windows Command Prompt) for all development.

#### Verify Build Environment
```bash
# Check compiler
gcc --version
g++ --version

# Check CMake
cmake --version

# Check SFML installation
pkg-config --modversion sfml-all 2>/dev/null || echo "SFML not in pkg-config"
ls /usr/local/lib/libsfml*
```

### 2. Clone and Configure DeerPortal

#### Clone Repository
```bash
# Navigate to desired location
cd /home/$USER  # or /cygdrive/c/Users/$USER/Documents

# Clone DeerPortal
git clone https://github.com/deerportal/deerportal.git
cd deerportal
```

#### Build Configuration
```bash
# Create build directory
mkdir build && cd build

# Configure with custom SFML path
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DSFML_DIR=/usr/local/lib/cmake/SFML \
      ..

# Alternative: Debug build
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DSFML_DIR=/usr/local/lib/cmake/SFML \
      ..
```

#### CMake Configuration Verification
Look for successful configuration:
```
-- The CXX compiler identification is GNU
-- Found SFML 3.0.0 in /usr/local/lib/cmake/SFML
-- Configuring done
-- Generating done
-- Build files have been written to: .../deerportal/build
```

### 3. Compilation

#### Build DeerPortal
```bash
# Parallel compilation
make -j$(nproc)

# Monitor build progress
make VERBOSE=1

# Successful build output
[100%] Built target DeerPortal
```

#### Verify Executable
```bash
# Check file type
file DeerPortal.exe
# Output: DeerPortal.exe: PE32+ executable (console) x86-64, for MS Windows

# Check dependencies
ldd DeerPortal.exe
# Should show cygwin1.dll and SFML libraries
```

## Advanced Cygwin Configuration

### 1. Environment Optimization

#### Cygwin-specific .bashrc
```bash
# Add to ~/.bashrc
export PATH="/usr/local/bin:$PATH"
export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"
export LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# DeerPortal development aliases
alias dp-configure='cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/local ..'
alias dp-debug='cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/usr/local ..'
alias dp-build='make -j$(nproc)'
alias dp-clean='make clean && rm -f CMakeCache.txt'

# Cygwin optimizations
export CYGWIN="winsymlinks:nativestrict"
```

#### Windows-Cygwin Path Integration
```bash
# Access Windows drives
cd /cygdrive/c/Users/$USER/Documents

# Convert between Windows and Cygwin paths
cygpath -w /home/$USER/deerportal
# Output: C:\cygwin64\home\username\deerportal

cygpath -u "C:\Users\username\Documents"
# Output: /cygdrive/c/Users/username/Documents
```

### 2. Development Tools

#### X11 Environment (Optional)
```bash
# Install X11 packages via setup-x86_64.exe:
# - xinit
# - xorg-server
# - xterm

# Start X11 server
startxwin-server &

# Launch X11 applications
export DISPLAY=:0.0
xterm &
```

#### Advanced Debugging
```bash
# Install GDB
# Add via setup-x86_64.exe: gdb

# Debug session
gdb ./DeerPortal.exe

# GDB with better TUI
gdb -tui ./DeerPortal.exe
```

## Asset and Runtime Management

### 1. File System Integration

#### Asset Path Handling
Cygwin uses Unix-style paths internally:
```bash
# DeerPortal asset structure
./DeerPortal.exe          # Cygwin executable
./assets/                 # Unix-style asset paths
├── img/
├── audio/
├── fonts/
└── data/
```

#### Path Conversion Utilities
```bash
# Convert Windows paths to Cygwin
cygpath -u "C:\path\to\assets"

# Convert Cygwin paths to Windows
cygpath -w /home/user/deerportal/assets
```

### 2. Running DeerPortal

#### From Cygwin Terminal
```bash
# Direct execution
./DeerPortal.exe

# With test mode
./DeerPortal.exe --test

# Check library dependencies
ldd DeerPortal.exe | grep -E "(cygwin|sfml)"
```

#### From Windows Explorer
Double-clicking the executable from Windows Explorer requires:
1. **Cygwin1.dll** in PATH
2. **SFML libraries** accessible
3. **Assets directory** in correct relative location

## Distribution and Packaging

### 1. Cygwin-Dependent Distribution

#### Required Dependencies
```bash
# List all dependencies
ldd DeerPortal.exe

# Typical Cygwin dependencies:
# - cygwin1.dll (core Cygwin runtime)
# - cygsfml-*.dll (SFML libraries)
# - cyggcc_s-seh-1.dll (GCC runtime)
# - cygstdc++-6.dll (C++ standard library)
```

#### Create Distribution Package
```bash
#!/bin/bash
# create-cygwin-package.sh

DIST_NAME="DeerPortal-0.9.0-Cygwin"
mkdir -p "$DIST_NAME"

# Copy executable
cp DeerPortal.exe "$DIST_NAME/"

# Copy assets
cp -r assets "$DIST_NAME/"

# Copy required Cygwin DLLs
cp /usr/bin/cygwin1.dll "$DIST_NAME/"
cp /usr/bin/cyggcc_s-seh-1.dll "$DIST_NAME/"
cp /usr/bin/cygstdc++-6.dll "$DIST_NAME/"

# Copy SFML libraries
cp /usr/local/bin/cygsfml-*.dll "$DIST_NAME/" 2>/dev/null || true

# Create launcher script
cat > "$DIST_NAME/run-deerportal.bat" << 'EOF'
@echo off
set PATH=%~dp0;%PATH%
DeerPortal.exe
EOF

# Create archive
tar -czf "$DIST_NAME.tar.gz" "$DIST_NAME"

echo "Package created: $DIST_NAME.tar.gz"
```

### 2. Standalone Distribution (Advanced)

#### Static Linking Attempt
```bash
# Try static linking (may not work with all SFML features)
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++" \
      -DSFML_STATIC_LIBRARIES=ON \
      ..
```

#### Wine Compatibility Testing
```bash
# Test on Linux with Wine (optional)
wine DeerPortal.exe
```

## Integration with Windows Tools

### 1. IDE Integration

#### Code::Blocks with Cygwin
```bash
# Install Code::Blocks via setup-x86_64.exe
# Configure compiler:
# - Compiler: GNU GCC Compiler
# - Installation directory: /usr
```

#### Eclipse CDT
```bash
# Download Eclipse CDT for Windows
# Configure:
# - Toolchain: Cygwin GCC
# - Build Command: make -j$(nproc)
```

### 2. Version Control Integration

#### Git Configuration
```bash
# Configure Git for cross-platform compatibility
git config --global core.autocrlf input
git config --global core.filemode false

# Handle Windows line endings
git config --global core.safecrlf true
```

## Performance Considerations

### 1. Compilation Performance

#### Optimization Flags
```bash
# Aggressive optimization
export CXXFLAGS="-O3 -march=native -mtune=native"

# Link-time optimization
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -flto" \
      ..
```

#### Parallel Builds
```bash
# Use all CPU cores
make -j$(nproc)

# Limit parallel jobs (if system becomes unresponsive)
make -j4
```

### 2. Runtime Performance

#### Cygwin Performance Notes
- **DLL Overhead**: cygwin1.dll adds some runtime overhead
- **POSIX Emulation**: Full POSIX compatibility has performance cost
- **File System**: Cygwin file operations are slower than native Windows

#### Optimization Tips
```bash
# Disable unnecessary POSIX features
export CYGWIN="nodosfilewarning"

# Use native Windows paths when possible
# (though this breaks POSIX compatibility)
```

## Troubleshooting

### Common Issues

#### "Cannot execute binary file"
```bash
# Ensure you're in Cygwin terminal, not Windows Command Prompt
echo $SHELL
# Should output: /bin/bash
```

#### SFML Headers Not Found
```bash
# Check SFML installation
ls /usr/local/include/SFML/

# Add include path if needed
export CXXFLAGS="-I/usr/local/include"
```

#### Missing DLL Errors
```bash
# Check which DLLs are missing
ldd DeerPortal.exe | grep "not found"

# Add Cygwin bin to Windows PATH
export PATH="/usr/bin:$PATH"
```

#### Build Failures
```bash
# Clean rebuild
make clean
rm -rf CMakeFiles CMakeCache.txt

# Reconfigure with verbose output
cmake -DCMAKE_VERBOSE_MAKEFILE=ON \
      -DCMAKE_PREFIX_PATH=/usr/local \
      ..
```

### Performance Issues

#### Slow File Operations
```bash
# Disable Windows Defender real-time scanning for Cygwin directory
# Or add C:\cygwin64 to Windows Defender exclusions
```

#### Memory Usage
```bash
# Monitor memory usage
ps aux | grep DeerPortal

# Limit compiler memory usage
export MAKEFLAGS="-j2"  # Reduce parallel jobs
```

## Comparison with Other Methods

### Cygwin vs MSYS2
- **Pros**: Full POSIX compliance, mature ecosystem, X11 support
- **Cons**: Larger runtime overhead, DLL dependency, slower file operations

### Cygwin vs Visual Studio
- **Pros**: Unix familiarity, shell scripting, traditional toolchain
- **Cons**: No integrated debugging GUI, Windows integration challenges

## Migration and Compatibility

### From Linux/Unix Systems
```bash
# Most Unix scripts work unchanged
# Build systems (autotools, make) work as expected
# Shell scripting compatibility is excellent
```

### Windows Integration
```bash
# Access Windows registry (advanced)
regtool -q get '\HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\ProgramFilesDir'

# Windows service interaction
net start "service-name"
```

## Next Steps

1. **Complete SFML compilation** and DeerPortal build
2. **Test executable** in both Cygwin and Windows environments
3. **Create distribution** package with required DLLs
4. **Set up development** workflow with preferred tools
5. **Consider performance** implications for your use case

---

**Cygwin Version**: 3.4.x  
**GCC Version**: 11.x+  
**SFML Version**: 3.0.x (compiled from source)  
**POSIX Compliance**: Full  
**Windows Integration**: Via cygwin1.dll 